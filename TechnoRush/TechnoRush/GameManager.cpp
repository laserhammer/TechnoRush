
#include "GameManager.h"
#include "InputManager.h"
#include "DebugCameraController.h"
#include "Camera.h"
#include "GameEntity.h"


GameManager::GameManager()
{
	_debugCamera = new Camera;
	_debugCamera->cullingMask(MAXINT32);
	DebugCameraController::init(_debugCamera);

	_currentGameState = Menu;
}

GameManager::~GameManager()
{
	delete _debugCamera;
}

void GameManager::Update(float dt)
{
	InputManager::Update();
	DebugCameraController::Update();

	//Update FSM
	UpdateFSM();
}

Camera* GameManager::mainCamera()
{
	return _debugCamera;
}

void GameManager::RenderScene(GameEntity** entities, int numEntities, ID3D11RenderTargetView* renderTargetView, ID3D11DepthStencilView* depthStencilView, ID3D11DeviceContext* deviceContext)
{
	_debugCamera->RenderScene(entities, numEntities, renderTargetView, depthStencilView, deviceContext);
}

// Private Member functions
void GameManager::UpdateFSM()
{
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

void GameManager::ChangeGamestate(GameState newGameState)
{
	_currentGameState = newGameState;
	switch (_currentGameState)
	{
	case Menu:
		_debugCamera->cullingMask(3);
		break;
	case Play:
		_debugCamera->cullingMask(1);
		break;
	case Pause:
		_debugCamera->cullingMask(2);
		break;
	default:
		break;
	}
}

void GameManager::QuitGame()
{

}