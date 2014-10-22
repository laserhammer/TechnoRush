#include <Windows.h>
#include <d3dcompiler.h>
#include <ctime>
#include <iostream>
#include "Game.h"
#include "GameEntity.h"
#include "WICTextureLoader.h"

#pragma region Win32 Entry Point (WinMain)

// Win32 Entry Point
int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE prevInstance,
				   PSTR cmdLine, int showCmd)
{
	// Enable run-time memory check for debug builds.
#if defined(DEBUG) | defined(_DEBUG)
	_CrtSetDbgFlag( _CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF );
#endif

	// Make the game, initialize and run
	Game game(hInstance);
	
	if( !game.Init() )
		return 0;
	
	return game.Run();
}

#pragma endregion

#pragma region Constructor / Destructor

Game::Game(HINSTANCE hInstance) : DirectXGame(hInstance)
{
	// Set up our custom caption and window size
	windowCaption = L"TechnoRush";
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
	delete(material);
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
	if( !DirectXGame::Init() )
		return false;

	// Set up buffers and such
	LoadShadersAndInputLayout();
	CreateGeometryBuffers();

	// Set up view matrix (camera)
	// In an actual game, update this when the camera moves (every frame)
	XMVECTOR position	= XMVectorSet(0, 0, -5, 0);
	XMVECTOR target		= XMVectorSet(0, 0, 0, 0);
	XMVECTOR up			= XMVectorSet(0, 1, 0, 0);
	XMMATRIX V			= XMMatrixLookAtLH(position, target, up);
	XMStoreFloat4x4(&viewMatrix, XMMatrixTranspose(V));

	return true;
}


// Creates the vertex and index buffers for a single triangle
void Game::CreateGeometryBuffers()
{
	/*
	Vertex vertices[3];
	Vertex verticesGreen[4];

	textureView = nullptr;
	samplerState = nullptr;

	entities = std::vector <GameEntity*>();

	XMFLOAT4 red	= XMFLOAT4(1.0f, 0.0f, 0.0f, 1.0f);
	XMFLOAT4 green	= XMFLOAT4(0.0f, 1.0f, 0.0f, 1.0f);
	XMFLOAT4 blue	= XMFLOAT4(0.0f, 0.0f, 1.0f, 1.0f);
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
	
	material = new Material(textureView, samplerState);

	std::srand(time(0));

	// Set up the vertices
	vertices[0] = { XMFLOAT3(+0.0f, +1.0f, +0.0f), white, XMFLOAT2(0.5f, 0.0f) };
	vertices[1] = { XMFLOAT3(-0.5f, +0.0f, +0.0f), white, XMFLOAT2(0.0f, 1.0f) };
	vertices[2] = { XMFLOAT3(+0.5f, +0.0f, +0.0f), white, XMFLOAT2(1.f, 1.0f) };

	// Set up the indices
	UINT indices[] = { 0, 2, 1 };
	entities.push_back(new GameEntity(vertices, 3, indices, 3, device, vsConstantBuffer, &dataToSendToVSConstantBuffer, material));

	//// Do the same thing but now for green triangles
	verticesGreen[0] = { XMFLOAT3(-0.5f, +0.5f, +0.0f), white, XMFLOAT2(0.0f, 0.0f) };
	verticesGreen[1] = { XMFLOAT3(+0.5f, +0.5f, +0.0f), white, XMFLOAT2(1.0f, 0.0f) };
	verticesGreen[2] = { XMFLOAT3(+0.5f, -0.5f, +0.0f), white, XMFLOAT2(1.0f, 1.0f) };
	verticesGreen[3] = { XMFLOAT3(-0.5f, -0.5f, +0.0f), white, XMFLOAT2(0.0f, 1.0f) };
	UINT indiciesGreen[] = { 0, 1, 2, 0, 2, 3 };
	entities.push_back(new GameEntity(verticesGreen, 4, indiciesGreen, 6, device, vsConstantBuffer, &dataToSendToVSConstantBuffer, material));
	*/
}

// Loads shaders from compiled shader object (.cso) files, and uses the
// vertex shader to create an input layout which is needed when sending
// vertex data to the device
void Game::LoadShadersAndInputLayout()
{
	/*
	// Set up the vertex layout description
	// This has to match the vertex input layout in the vertex shader
	// We can't set up the input layout yet since we need the actual vert shader
	D3D11_INPUT_ELEMENT_DESC vertexDesc[] =
	{
		{"POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT,	0, 0,	D3D11_INPUT_PER_VERTEX_DATA, 0},
		{"COLOR",    0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, 12,	D3D11_INPUT_PER_VERTEX_DATA, 0},
		{"TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT,		0, 28,	D3D11_INPUT_PER_VERTEX_DATA, 0}
	};

	// Load Vertex Shader --------------------------------------
	ID3DBlob* vsBlob;
	D3DReadFileToBlob(L"VertexShader.cso", &vsBlob);

	// Create the shader on the device
	HR(device->CreateVertexShader(
		vsBlob->GetBufferPointer(),
		vsBlob->GetBufferSize(),
		NULL,
		&vertexShader));

	// Before cleaning up the data, create the input layout
	HR(device->CreateInputLayout(
		vertexDesc,
		ARRAYSIZE(vertexDesc),
		vsBlob->GetBufferPointer(),
		vsBlob->GetBufferSize(),
		&inputLayout));

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
	cBufferDesc.ByteWidth           = sizeof(dataToSendToVSConstantBuffer);
	cBufferDesc.Usage				= D3D11_USAGE_DEFAULT;
	cBufferDesc.BindFlags			= D3D11_BIND_CONSTANT_BUFFER;
	cBufferDesc.CPUAccessFlags		= 0;
	cBufferDesc.MiscFlags			= 0;
	cBufferDesc.StructureByteStride = 0;
	HR(device->CreateBuffer(
		&cBufferDesc,
		NULL,
		&vsConstantBuffer));
		*/
}

#pragma endregion

#pragma region Window Resizing

// Handles resizing the window and updating our projection matrix to match
void Game::OnResize()
{
	// Handle base-level DX resize stuff
	DirectXGame::OnResize();

	// Update our projection matrix since the window size changed
	XMMATRIX P = XMMatrixPerspectiveFovLH(
		0.5f * 3.1415926535f,
		AspectRatio(),
		0.1f,
		100.0f);
	XMStoreFloat4x4(&projectionMatrix, XMMatrixTranspose(P));
}
#pragma endregion

#pragma region Game Loop

// Updates the local constant buffer and 
// push it to the buffer on the device
void Game::UpdateScene(float dt)
{
	/*
	// Update entities
	for each (GameEntity* entity in entities)
	{
		entity->Update(dt);
	}

	// Update local constant buffer data
	dataToSendToVSConstantBuffer.view		= viewMatrix;
	dataToSendToVSConstantBuffer.projection	= projectionMatrix;
	*/
}

// Clear the screen, redraw everything, present
void Game::DrawScene()
{
	/*
	const float color[4] = {0.4f, 0.6f, 0.75f, 0.0f};

	// Clear the buffer
	deviceContext->ClearRenderTargetView(renderTargetView, color);
	deviceContext->ClearDepthStencilView(
		depthStencilView, 
		D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL,
		1.0f,
		0);

	// Set up the input assembler
	deviceContext->IASetInputLayout(inputLayout);
	deviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

	for each (GameEntity* entity in entities)
	{
		// Set the current vertex and pixel shaders, as well the constant buffer for the vert shader
		deviceContext->VSSetShader(vertexShader, NULL, 0);
		deviceContext->VSSetConstantBuffers(
			0,	// Corresponds to the constant buffer's register in the vertex shader
			1,
			&vsConstantBuffer);
		deviceContext->PSSetShader(pixelShader, NULL, 0);

		entity->Draw(deviceContext);
	}

	// Present the buffer
	HR(swapChain->Present(0, 0));
	*/
}

#pragma endregion
