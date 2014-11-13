

#include <Windows.h>
#include <d3dcompiler.h>
#include <ctime>
#include <iostream>
#include <cstdlib>
#include <fstream>
#include "Game.h"
#include "GameEntity.h"
#include "Camera.h"
#include "WICTextureLoader.h"
#include "GameManager.h"
#include "InputManager.h"
#include "WorldManager.h"
#include "AssetLoader.h"


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
	windowCaption = L"Demo DX11 Game";
	windowWidth = 800;
	windowHeight = 600;
}

Game::~Game()
{
	// Release all of the D3D stuff that's still hanging out
	ReleaseMacro(vertexShader);
	ReleaseMacro(pixelShader);
	ReleaseMacro(vsConstantBuffer);
	ReleaseMacro(inputLayout);
	ReleaseMacro(textureView);
	ReleaseMacro(samplerState);
	delete(material);
	delete(gameManager);
	delete(worldManager);
	while (entities.size() > 0)
	{
		GameEntity* entity = entities[entities.size() - 1];
		delete(entity);
		entities.pop_back();
	}
}

#pragma endregion

#pragma region Initialization

// Initializes the base class (including the window and D3D),
// sets up our geometry and loads the shaders (among other things)
bool Game::Init()
{
	gameManager = new GameManager;
	worldManager = new WorldManager();
	//camera = new Camera();

	if (!DirectXGame::Init())
		return false;

	// Set up buffers and such
	LoadShadersAndInputLayout();
	CreateGeometryBuffers();
	
	return true;
}


// Creates the vertex and index buffers for a single triangle
void Game::CreateGeometryBuffers()
{
	std::vector<Vertex> cubeVec(8);

	textureView = nullptr;
	samplerState = nullptr;

	entities = std::vector <GameEntity*>();

	XMFLOAT4 red = XMFLOAT4(1.0f, 0.0f, 0.0f, 1.0f);
	XMFLOAT4 green = XMFLOAT4(0.0f, 1.0f, 0.0f, 1.0f);
	XMFLOAT4 blue = XMFLOAT4(0.0f, 0.0f, 1.0f, 1.0f);
	XMFLOAT4 white = XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f);

	D3D11_SAMPLER_DESC desc;
	desc.Filter = D3D11_FILTER_MIN_MAG_MIP_POINT;
	desc.AddressU = D3D11_TEXTURE_ADDRESS_WRAP;
	desc.AddressV = D3D11_TEXTURE_ADDRESS_WRAP;
	desc.AddressW = D3D11_TEXTURE_ADDRESS_WRAP;
	desc.BorderColor[0] = 0;
	desc.BorderColor[1] = 0;
	desc.BorderColor[2] = 0;
	desc.BorderColor[3] = 0;
	desc.ComparisonFunc = D3D11_COMPARISON_ALWAYS;
	desc.MaxAnisotropy = 0;
	desc.MaxLOD = 0;
	desc.MinLOD = 0;
	desc.MipLODBias = 0;

	device->CreateSamplerState(&desc, &samplerState);

	HRESULT result = CreateWICTextureFromFile(device, deviceContext, L"../Textures/Test_card.png", 0, &textureView);
	std::cout << result;

	material = new Material(textureView, samplerState, vertexShader, vsConstantBuffer, pixelShader, inputLayout);
	

	std::srand((unsigned int)time(0));

	cubeVec[0] = { XMFLOAT3(-0.5f, +0.5f, +0.0f), white, XMFLOAT2(0.0f, 0.0f) };
	cubeVec[1] = { XMFLOAT3(+0.5f, +0.5f, +0.0f), white, XMFLOAT2(1.0f, 0.0f) };
	cubeVec[2] = { XMFLOAT3(+0.5f, -0.5f, +0.0f), white, XMFLOAT2(1.0f, 1.0f) };
	cubeVec[3] = { XMFLOAT3(-0.5f, -0.5f, +0.0f), white, XMFLOAT2(0.0f, 1.0f) };
	cubeVec[4] = { XMFLOAT3(-0.5f, +0.5f, -0.5f), white, XMFLOAT2(0.0f, 0.0f) };
	cubeVec[5] = { XMFLOAT3(+0.5f, +0.5f, -0.5f), white, XMFLOAT2(1.0f, 0.0f) };
	cubeVec[6] = { XMFLOAT3(+0.5f, -0.5f, -0.5f), white, XMFLOAT2(1.0f, 1.0f) };
	cubeVec[7] = { XMFLOAT3(-0.5f, -0.5f, -0.5f), white, XMFLOAT2(0.0f, 1.0f) };

	std::vector<UINT> cubeInd = { 2, 1, 0, 3, 2, 0, 1, 4, 0, 5, 4, 1, 6, 5, 1, 1, 2, 6, 7, 6, 2, 7, 2, 3, 0, 4, 7, 7, 3, 0, 4, 5, 6, 6, 7, 4 };

	


	for (int i = 0; i < 45; i++)
	{
		entities.push_back(new GameEntity(cubeVec, cubeInd, device, &dataToSendToVSConstantBuffer, material));
	}
	worldManager->getEntities(&entities);

	// This is the stream to open up the file
	ifstream in_Stream1;
	in_Stream1.open("PlayerShip.obj");
	entities.push_back(AssetLoader::LoadOBJ(device, &dataToSendToVSConstantBuffer, material, in_Stream1)); // The stream is for the OBJ to me loaded
	entities[entities.size() - 1]->position(XMFLOAT4(0.0f, 0.0f, -3.0f, 0.0f));
	

	in_Stream1.close();
	in_Stream1.open("Cube2.obj");

	//last one is the UI

	entities.push_back(AssetLoader::LoadOBJ(device, &dataToSendToVSConstantBuffer, material, in_Stream1)); // The stream is for the OBJ to me loaded
	entities[entities.size()-1]->position(XMFLOAT4(-4.5f, 3.5f, 0.0f, 0.0f));
	entities[entities.size() - 1]->layer(2);

	// Close the stream when we're done with it
	in_Stream1.close();

	

}

// Loads shaders from compiled shader object (.cso) files, and uses the
// vertex shader to create an input layout which is needed when sending
// vertex data to the device
void Game::LoadShadersAndInputLayout()
{
	// Load Vertex Shader --------------------------------------
	ID3DBlob* vsBlob;
	D3DReadFileToBlob(L"VertexShader.cso", &vsBlob);

	// Create the shader on the device
	HR(device->CreateVertexShader(
		vsBlob->GetBufferPointer(),
		vsBlob->GetBufferSize(),
		NULL,
		&vertexShader));

	CreateInputLayoutDescFromVertexShaderSignature(vsBlob, &inputLayout);

	// Clean up
	ReleaseMacro(vsBlob);

	// Load Pixel Shader ---------------------------------------
	ID3DBlob* psBlob;
	D3DReadFileToBlob(L"PixelShader.cso", &psBlob);

	// Create the shader on the device
	HR(device->CreatePixelShader(
		psBlob->GetBufferPointer(),
		psBlob->GetBufferSize(),
		NULL,
		&pixelShader));

	// Clean up
	ReleaseMacro(psBlob);

	// Constant buffers ----------------------------------------
	D3D11_BUFFER_DESC cBufferDesc;
	cBufferDesc.ByteWidth = sizeof(dataToSendToVSConstantBuffer);
	cBufferDesc.Usage = D3D11_USAGE_DEFAULT;
	cBufferDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	cBufferDesc.CPUAccessFlags = 0;
	cBufferDesc.MiscFlags = 0;
	cBufferDesc.StructureByteStride = 0;
	HR(device->CreateBuffer(
		&cBufferDesc,
		NULL,
		&vsConstantBuffer));
}

// From http://takinginitiative.wordpress.com/2011/12/11/directx-1011-basic-shader-reflection-automatic-input-layout-creation/
void Game::CreateInputLayoutDescFromVertexShaderSignature(ID3DBlob* shaderBlob, ID3D11InputLayout** inputLayout)
{
	// Reflect shader info
	ID3D11ShaderReflection* vertexShaderReflection = NULL;
	HR(D3DReflect(shaderBlob->GetBufferPointer(), shaderBlob->GetBufferSize(), IID_ID3D11ShaderReflection, (void**)&vertexShaderReflection));

	// Get shader info
	D3D11_SHADER_DESC shaderDesc;
	vertexShaderReflection->GetDesc(&shaderDesc);

	// Read input layout description from shader info
	std::vector<D3D11_INPUT_ELEMENT_DESC> inputLayoutDesc;
	for (UINT32 i = 0; i < shaderDesc.InputParameters; ++i)
	{
		D3D11_SIGNATURE_PARAMETER_DESC paramDesc;
		vertexShaderReflection->GetInputParameterDesc(i, &paramDesc);

		// fill out input element desc
		D3D11_INPUT_ELEMENT_DESC elementDesc;
		elementDesc.SemanticName = paramDesc.SemanticName;
		elementDesc.SemanticIndex = paramDesc.SemanticIndex;
		elementDesc.InputSlot = 0;
		elementDesc.AlignedByteOffset = D3D11_APPEND_ALIGNED_ELEMENT;
		elementDesc.InputSlotClass = D3D11_INPUT_PER_VERTEX_DATA;
		elementDesc.InstanceDataStepRate = 0;

		// determine DXGI format
		if (paramDesc.Mask == 1)
		{
			if (paramDesc.ComponentType == D3D_REGISTER_COMPONENT_UINT32) elementDesc.Format = DXGI_FORMAT_R32_UINT; 
			else if (paramDesc.ComponentType == D3D_REGISTER_COMPONENT_SINT32) elementDesc.Format = DXGI_FORMAT_R32_SINT;
			else if (paramDesc.ComponentType == D3D_REGISTER_COMPONENT_FLOAT32) elementDesc.Format = DXGI_FORMAT_R32_FLOAT;
		}
		else if (paramDesc.Mask <= 3)
		{
			if (paramDesc.ComponentType == D3D_REGISTER_COMPONENT_UINT32) elementDesc.Format = DXGI_FORMAT_R32G32_UINT;
			else if (paramDesc.ComponentType == D3D_REGISTER_COMPONENT_SINT32) elementDesc.Format = DXGI_FORMAT_R32G32_SINT;
			else if (paramDesc.ComponentType == D3D_REGISTER_COMPONENT_FLOAT32) elementDesc.Format = DXGI_FORMAT_R32G32_FLOAT;
		}
		else if (paramDesc.Mask <= 7)
		{
			if (paramDesc.ComponentType == D3D_REGISTER_COMPONENT_UINT32) elementDesc.Format = DXGI_FORMAT_R32G32B32_UINT;
			else if (paramDesc.ComponentType == D3D_REGISTER_COMPONENT_SINT32) elementDesc.Format = DXGI_FORMAT_R32G32B32_SINT;
			else if (paramDesc.ComponentType == D3D_REGISTER_COMPONENT_FLOAT32) elementDesc.Format = DXGI_FORMAT_R32G32B32_FLOAT;
		}
		else if (paramDesc.Mask <= 15)
		{
			if (paramDesc.ComponentType == D3D_REGISTER_COMPONENT_UINT32) elementDesc.Format = DXGI_FORMAT_R32G32B32A32_UINT;
			else if (paramDesc.ComponentType == D3D_REGISTER_COMPONENT_SINT32) elementDesc.Format = DXGI_FORMAT_R32G32B32A32_SINT;
			else if (paramDesc.ComponentType == D3D_REGISTER_COMPONENT_FLOAT32) elementDesc.Format = DXGI_FORMAT_R32G32B32A32_FLOAT;
		}

		// save element desc
		inputLayoutDesc.push_back(elementDesc);
	}

	// Try to create Input Layout
	HR(device->CreateInputLayout(&inputLayoutDesc[0], inputLayoutDesc.size(), shaderBlob->GetBufferPointer(), shaderBlob->GetBufferSize(), inputLayout));

	// Free allocation shader reflection memory
	ReleaseMacro(vertexShaderReflection);
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

	if (gameManager->getGameState() == Play)
	{
		worldManager->Update(dt);

		// Update entities
		for each (GameEntity* entity in entities)
		{
			entity->Update(dt);
		}
	}

	// Update local constant buffer data
	//dataToSendToVSConstantBuffer.view = gameManager->mainCamera()->view();
	//dataToSendToVSConstantBuffer.projection = gameManager->mainCamera()->projection();
}

// Clear the screen, redraw everything, present
void Game::DrawScene()
{
	gameManager->RenderScene(&entities[0], entities.size(), renderTargetView, depthStencilView, deviceContext, dataToSendToVSConstantBuffer.view, dataToSendToVSConstantBuffer.projection);
	// Present the buffer
	HR(swapChain->Present(0, 0));
}

#pragma endregion

#pragma region Mouse Input

// These methods don't do much currently, but can be used for mouse-related input

void Game::OnMouseDown(WPARAM btnState, int x, int y)
{
	InputManager::OnMouseDown(x, y);
	SetCapture(hMainWnd);
}

void Game::OnMouseUp(WPARAM btnState, int x, int y)
{
	InputManager::OnMouseUp(x, y);
	ReleaseCapture();
}

void Game::OnMouseMove(WPARAM btnState, int x, int y)
{
	InputManager::OnMouseMove(x, y);
}
#pragma endregion