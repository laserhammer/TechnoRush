
#include "GameManager.h"
#include "InputManager.h"
#include "DebugCameraController.h"
#include "Camera.h"
#include <iostream>


GameManager::GameManager()
{
	_camera = new Camera;
	DebugCameraController::init(_camera);
}

GameManager::~GameManager()
{
	delete _camera;
}

void GameManager::Update(float dt)
{
	InputManager::Update();
	DebugCameraController::Update();
}

// Accessors
Camera* GameManager::mainCamera()
{
	return _camera;
}