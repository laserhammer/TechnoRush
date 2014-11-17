#pragma once

#include <DirectXMath.h>
#include <vector>
#include "DirectXGame.h"

// Include run-time memory checking in debug builds
#if defined(DEBUG) || defined(_DEBUG)
#define _CRTDBG_MAP_ALLOC
#include <crtdbg.h>
#endif

// For DirectX Math
using namespace DirectX;

class GameEntity;
class Material;
class Mesh;
class Camera;
class GameManager;
class WorldManager;

// Struct to match vertex shader's constant buffer
// You update one of these locally, then push it to the corresponding
// constant buffer on the device when it needs to be updated
struct VSConstantBufferLayout
{
	XMFLOAT4X4 world;
	XMFLOAT4X4 view;
	XMFLOAT4X4 projection;
};

struct VSLightConstantBuffer
{
	XMFLOAT4 lightPos;
};
//struct PSConstantBufferLayout{};

// Demo class which extends the base DirectXGame class
class Game : public DirectXGame
{
public:
	Game(HINSTANCE hInstance);
	~Game();

	// Overrides for base level methods
	bool Init();
	void OnResize();
	void UpdateScene(float dt);
	void DrawScene();

	// For handing mouse input
	void OnMouseDown(WPARAM btnState, int x, int y);
	void OnMouseUp(WPARAM btnState, int x, int y);
	void OnMouseMove(WPARAM btnState, int x, int y);

	

private:
	// Initialization for our "game" demo
	void CreateGeometryBuffers();
	void LoadShadersAndInputLayout();
	void LoadTexture(const wchar_t* path);
	void CreateInputLayoutDescFromVertexShaderSignature(ID3DBlob* shaderBlob, ID3D11InputLayout** inputLayout);
private:

	std::vector<GameEntity*> entities;

	//Camera* camera;

	// Our basic shaders for this example
	ID3D11PixelShader* pixelShader;
	ID3D11VertexShader* vertexShader;

	// Texture data
	//ID3D11Resource* texture;
	ID3D11ShaderResourceView* textureView;
	ID3D11SamplerState* samplerState;
	Material* material;

	// A few more odds and ends we'll need
	ID3D11InputLayout* inputLayout;
	ID3D11Buffer* vsConstantBuffer;
	VSConstantBufferLayout dataToSendToVSConstantBuffer;
	ID3D11Buffer* lightBuffer;
	VSLightConstantBuffer lightData;


	// The matrices to go from model space
	// to screen space
	XMFLOAT4X4 viewMatrix;
	XMFLOAT4X4 projectionMatrix;

	// Keeps track of the old mouse position.  Useful for 
	// determining how far the mouse moved in a single frame.
	POINT prevMousePos;

	GameManager* gameManager;
	WorldManager* worldManager;
};