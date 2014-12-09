

#include <Windows.h>
#include <d3dcompiler.h>
#include <ctime>
#include <iostream>
#include <cstdlib>
#include <fstream>
#include "Game.h"
#include "GameEntity.h"
#include "GameManager.h"


#pragma region Win32 Entry Point (WinMain)

// Win32 Entry Point
int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE prevInstance,
	PSTR cmdLine, int showCmd)
{
	// Enable run-time memory check for debug builds.
#if defined(DEBUG) | defined(_DEBUG)
	_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);
#endif

	// Make the game, initialize and run
	Game game(hInstance);

	if (!game.Init())
		return 0;

	return game.Run();
}

#pragma endregion

#pragma region Constructor / Destructor

Game::Game(HINSTANCE hInstance) : DirectXGame(hInstance)
{
	// Set up our custom caption and window size
	windowCaption = L"Technorush";
	windowWidth = 800;
	windowHeight = 600;
}

Game::~Game()
{
	delete(gameManager);
}

#pragma endregion

#pragma region Initialization

// Initializes the base class (including the window and D3D),
// sets up our geometry and loads the shaders (among other things)
bool Game::Init()
{
	gameManager = new GameManager;

	if (!DirectXGame::Init())
		return false;

	gameManager->LoadData(device, deviceContext);
	
	return true;
}

#pragma endregion

#pragma region Window Resizing

// Handles resizing the window and updating our projection matrix to match
void Game::OnResize()
{
	// Handle base-level DX resize stuff
	DirectXGame::OnResize();

	if (gameManager)
		gameManager->Resize(AspectRatio());
}
#pragma endregion

#pragma region Game Loop

// Updates the local constant buffer and 
// push it to the buffer on the device
void Game::UpdateScene(float dt)
{
	gameManager->Update(dt);
}

// Clear the screen, redraw everything, present
void Game::DrawScene()
{
	gameManager->RenderScene(backBufferRenderTargetView, depthStencilView, deviceContext);
	// Present the buffer
	HR(swapChain->Present(0, 0));
}

#pragma endregion

#pragma region Mouse Input

// These methods don't do much currently, but can be used for mouse-related input

void Game::OnMouseDown(WPARAM btnState, int x, int y)
{
	//InputManager::OnMouseDown(x, y);
	SetCapture(hMainWnd);
}

void Game::OnMouseUp(WPARAM btnState, int x, int y)
{
	//InputManager::OnMouseUp(x, y);
	ReleaseCapture();
}

void Game::OnMouseMove(WPARAM btnState, int x, int y)
{
	//InputManager::OnMouseMove(x, y);
}
#pragma endregion