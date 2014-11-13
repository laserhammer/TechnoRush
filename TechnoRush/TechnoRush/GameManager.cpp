
#include "GameManager.h"
#include "InputManager.h"
#include "DebugCameraController.h"
#include "Camera.h"
#include "GameEntity.h"


GameManager::GameManager()
{
	InitCameras();

	_currentGameState = Menu;

	_debugActive = false;
}

GameManager::~GameManager()
{
	delete _debugCamera;
	delete _gameCamera;
	delete _uiCamera;
}

void GameManager::Update(float dt)
{
	InputManager::Update();
	if (_debugActive)
		DebugCameraController::Update();

	//Update FSM
	UpdateFSM();
}

Camera* GameManager::mainCamera()
{
	if (_debugActive)
		return _debugCamera;
	else
		return _gameCamera;
}

void GameManager::RenderScene(GameEntity** entities, int numEntities, ID3D11RenderTargetView* renderTargetView, ID3D11DepthStencilView* depthStencilView, ID3D11DeviceContext* deviceContext, XMFLOAT4X4& viewData, XMFLOAT4X4& projectionData)
{
	if (_debugActive)
	{
		_debugCamera->RenderScene(entities, numEntities, renderTargetView, depthStencilView, deviceContext, viewData, projectionData);
	}
	else if (_currentGameState == Pause)
	{
		_uiCamera->RenderScene(entities, numEntities, renderTargetView, depthStencilView, deviceContext, viewData, projectionData);
	}
	else
	{
		_gameCamera->RenderScene(entities, numEntities, renderTargetView, depthStencilView, deviceContext, viewData, projectionData);
		_uiCamera->RenderScene(entities, numEntities, renderTargetView, depthStencilView, deviceContext, viewData, projectionData);
	}
}

void GameManager::Resize(float aspectRatio)
{
	_debugCamera->Resize(aspectRatio);
	_gameCamera->Resize(aspectRatio);
	_uiCamera->Resize(aspectRatio);
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
			break;
		case EndGame:
			ChangeGamestate(GameState::Menu);
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
	DebugCameraController::SetOrientation(-0.465, 0.0);
}

void GameManager::QuitGame()
{

}

void GameManager::InitCameras()
{
	_gameCamera = new Camera();
	_gameCamera->cullingMask(1);	//Can see world
	_gameCamera->SetViewParameters(XMFLOAT3(0.0f, 2.5f, -5.0f), XMFLOAT3(0.0f, 0.0f, 0.0f), XMFLOAT3(0.0f, 1.0f, 0.0f));

	_debugCamera = new Camera();
	_debugCamera->cullingMask(MAXINT32); // Can see EVERYTHING
	DebugCameraController::init(_debugCamera);
	// Set to (approximately) the gameCamera's starting position
	DebugCameraController::SetPosition(XMFLOAT3(0.0f, 2.5f, -5.0f));
	DebugCameraController::SetOrientation(-0.465, 0.0);

	_uiCamera = new Camera();
	_uiCamera->cullingMask(2);		//Can see ui
	_uiCamera->clearRenderTarget(false);
	_uiCamera->SetViewParameters(XMFLOAT3(0.0, 0.0, -5.0), XMFLOAT3(0.0, 0.0, 0.0), XMFLOAT3(0, 1, 0));
	//Not sure what's up with orthographic... can't get it to work
	_uiCamera->orthographic(true);
}