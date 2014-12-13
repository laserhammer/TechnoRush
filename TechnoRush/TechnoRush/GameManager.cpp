
#include "GameManager.h"
#include "InputManager.h"
#include "DebugCameraController.h"
#include "Camera.h"
#include "GameEntity.h"
#include "Game.h"
#include "WICTextureLoader.h"
#include "GameManager.h"
#include "InputManager.h"
#include "WorldManager.h"
#include "AssetLoader.h"
#include "UiManager.h"



GameManager::GameManager()
{
	InitCameras();

	_currentGameState = Menu;

	_debugActive = false;

	_worldManager = new WorldManager();

	_score = 0;
	_fov = DEFAULT_FOV;
}

GameManager::~GameManager()
{
	delete _debugCamera;
	delete _gameCamera;
	delete _uiCamera;
	delete _worldManager;
	AssetLoader::ReleaseAssets();
	UiManager::Release();
	while (_entities->size() > 0)
	{
		GameEntity* entity = (*_entities)[_entities->size() - 1];
		delete(entity);
		_entities->pop_back();
	 }
	delete _entities;
}

void GameManager::LoadData(ID3D11Device* device, ID3D11DeviceContext* deviceContext)
{
	AssetLoader::LoadAssets(device, deviceContext);

	_entities = new std::vector<GameEntity*>();

	int numObstacles = 46;
	_entities->resize(numObstacles);
	for (int i = 0; i < numObstacles; i+=2)
	{
		(*_entities)[i] = new GameEntity(AssetLoader::obstacleMat, AssetLoader::cube, &AssetLoader::vsData);
		(*_entities)[i + 1] = new GameEntity(AssetLoader::obstacleTowerMat, AssetLoader::tower, &AssetLoader::vsData);
	}
	

	_entities->resize(numObstacles + 1);
	//player
	(*_entities)[numObstacles] = new GameEntity(AssetLoader::playerMat, AssetLoader::player, &AssetLoader::vsData);
	(*_entities)[numObstacles]->position(XMFLOAT4(0.0f, 0.0f, -3.0f, 0.0f));

	_worldManager->getEntities(_entities);

	_entities->resize(numObstacles + 3);
	//Floor
	(*_entities)[numObstacles + 1] = new GameEntity(AssetLoader::floorMat, AssetLoader::floor, &AssetLoader::vsData);
	(*_entities)[numObstacles + 1]->scale(XMFLOAT4(100.0f, 1.0f, 100.0f, 0.0f));
	(*_entities)[numObstacles + 1]->position(XMFLOAT4(0.0f, -0.5f, 0.0f, 0.0f));
	_worldManager->SetFloor((*_entities)[numObstacles + 1]);

	//Background
	(*_entities)[numObstacles + 2] = new GameEntity(AssetLoader::backgroundMat, AssetLoader::quad, &AssetLoader::vsData);
	_gameCamera->SetBackground((*_entities)[numObstacles + 2]);

	UiManager::InitUi(_uiCamera);
	_gameCamera->InitPostProcRenderTarget(device);
}

void GameManager::Update(float dt)
{
	InputManager::Update();
	if (_debugActive)
		DebugCameraController::Update();

	//Update FSM
	UpdateFSM();
	XMFLOAT4 color;
	float speed = abs(_worldManager->getSpeed());
	_fov = DEFAULT_FOV - speed * 0.1f;
	switch (_currentGameState)
	{
	case GameState::Play:
		_worldManager->Update(dt);
		color = GetColorFromSpeed(speed - 0.04f);
		_gameCamera->Resize(_gameCamera->aspectRatio(), _fov);
		_score += speed*100;
		for each (GameEntity* entity in *_entities)
		{
			entity->color(color);
			entity->Update(dt);
		}
		break;
	case GameState::Menu:
		color = XMFLOAT4(0.0f, 0.0f, 0.0f, 1.0f);
		_gameCamera->Resize(_gameCamera->aspectRatio(), _fov);
		for each (GameEntity* entity in *_entities)
		{
			entity->color(color);
			entity->Update(dt);
		}
		break;
	case GameState::Pause:
		for each (GameEntity* entity in *_entities)
		{
			entity->Update(dt);
		}
		break;
	case GameState::EndGame:
		for each (GameEntity* entity in *_entities)
		{
			entity->Update(dt);
		}
		break;
	default:
		break;
	}
	UiManager::SetScore((int)_score);
	UiManager::Update(_currentGameState);
}

Camera* GameManager::mainCamera()
{
	if (_debugActive)
		return _debugCamera;
	else
		return _gameCamera;
}

void GameManager::RenderScene(ID3D11RenderTargetView *const *renderTargetView, ID3D11DepthStencilView* depthStencilView, ID3D11DeviceContext* deviceContext)
{
	int numEntities = _entities->size();
	Mesh* mesh = AssetLoader::player;
	if (_debugActive)
	{
		_debugCamera->RenderScene(&(*_entities)[0], numEntities, renderTargetView, 1, depthStencilView, deviceContext, AssetLoader::vsData.view, AssetLoader::vsData.projection);
	}
	else
	{
		_gameCamera->RenderScene(&(*_entities)[0], numEntities, renderTargetView, 1,  depthStencilView, deviceContext, AssetLoader::vsData.view, AssetLoader::vsData.projection);
		UiManager::DrawUi(renderTargetView, depthStencilView, deviceContext, AssetLoader::vsData.view, AssetLoader::vsData.projection);
	}
}

void GameManager::Resize(float aspectRatio)
{
	_debugCamera->Resize(aspectRatio, DEFAULT_FOV);
	_gameCamera->Resize(aspectRatio, _fov);
	_uiCamera->Resize(aspectRatio, DEFAULT_FOV);
}

// Private Member functions
void GameManager::UpdateFSM()
{
	if (InputManager::spaceKey && !InputManager::pspaceKey)
	{
		ToggleDebug();
	}

	if (InputManager::returnKey && !InputManager::preturnKey)
	{
		switch (_currentGameState)
		{
		case Menu:
			ChangeGamestate(GameState::Play);
			_score = 0;
			break;
		case EndGame:
			ChangeGamestate(GameState::Menu);
			_worldManager->setCollide(false);
			_worldManager->resetWorld();
			break;
		default:
			break;
		}
	}
	else if (InputManager::escKey && !InputManager::pescKey)
	{
		switch (_currentGameState)
		{
		case Menu:
			QuitGame();
		case Play:
			ChangeGamestate(GameState::Pause);
			break;
		case Pause:
			ChangeGamestate(GameState::Menu);
		default:
			break;
		}
	}

	if (_currentGameState == GameState::Play && _worldManager->getCollide())
	{
		ChangeGamestate(GameState::EndGame);
		
	}
}

GameState GameManager::getGameState()
{
	return _currentGameState;
}

void GameManager::ChangeGamestate(GameState newGameState)
{
	_currentGameState = newGameState;
	switch (_currentGameState)
	{
	case Menu:
		//_debugCamera->cullingMask(3);
		break;
	case Play:
		//_debugCamera->cullingMask(1);
		break;
	case Pause:
		//_debugCamera->cullingMask(2);
		break;
	default:
		break;
	}
}

void GameManager::ToggleDebug()
{
	_debugActive = !_debugActive;
	XMFLOAT3 pos = XMFLOAT3(_gameCamera->position().x, _gameCamera->position().y, _gameCamera->position().z);
	// Set to (approximately) the gameCamera's current position (which shouldn't change)
	DebugCameraController::SetPosition(pos);
	DebugCameraController::SetOrientation(-0.465f, 0.0f);
}

void GameManager::QuitGame()
{

}

void GameManager::InitCameras()
{
	_gameCamera = new Camera();
	_gameCamera->cullingMask(1);	//Can see world
	_gameCamera->SetViewParameters(XMFLOAT3(0.0f, 1.0f, -5.0f), XMFLOAT3(0.0f, 0.0f, 100.0f), XMFLOAT3(0.0f, 1.0f, 0.0f));

	_debugCamera = new Camera();
	_debugCamera->cullingMask(MAXINT32); // Can see EVERYTHING
	DebugCameraController::init(_debugCamera);
	// Set to (approximately) the gameCamera's starting position
	DebugCameraController::SetPosition(XMFLOAT3(0.0f, 2.5f, -5.0f));
	DebugCameraController::SetOrientation(-0.465f, 0.0f);

	_uiCamera = new Camera();
	_uiCamera->cullingMask(2);		//Can see ui
	_uiCamera->clearRenderTarget(false);
	_uiCamera->SetViewParameters(XMFLOAT3(0.0, 0.0, -5.0), XMFLOAT3(0.0, 0.0, 0.0), XMFLOAT3(0, 1, 0));
	//Not sure what's up with orthographic... can't get it to work
	_uiCamera->orthographic(true);
}

XMFLOAT4 GameManager::GetColorFromSpeed(float speed)
{
	float r = 0;
	float g = 0;
	float b = 0;

	if (speed < 0)
	{
		r = 0.412f - (speed / -0.04f) * (1.0f);
		r = r <= 0.0f ? 0.0f : r;
		g = 0.0f;
		b = 0.043f - (speed / -0.04f) * (1.0f);
		b = b <= 0.0f ? 0.0f : b;
	}
	else
	{
		r = 0.412f + (speed / 0.11f) * (.196f - 0.412f);
		r = r >= 1.0f ? 1.0f : r;
		g = 0.0f + (speed / 0.11f) * (0.807f - 0.0f);
		g = g >= 1.0f ? 1.0f : g;
		b = 0.043f + (speed / 0.11f) * (1.0f - 0.043f);
		b = b >= 1.0f ? 1.0f : b;
	}
	return XMFLOAT4(r, g, b, 1.0f);
}