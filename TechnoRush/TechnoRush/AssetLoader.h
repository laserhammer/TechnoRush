#pragma once

#include "GameEntity.h"
#include "Game.h"
#include "Material.h"
#include "ScrollingMaterial.h"
#include "DirectXGame.h"
#include "Mesh.h"

using namespace std;

struct VSConstantBufferLayout
{
	XMFLOAT4X4 world;
	XMFLOAT4X4 view;
	XMFLOAT4X4 projection;
	XMFLOAT4 color;
};

class AssetLoader
{
public:


	// This will load OBJs
	//static GameEntity* LoadOBJ(ID3D11Device *device, VSConstantBufferLayout *constantBufferLayout, Material* material, ifstream& in_Stream);

	static void LoadAssets(ID3D11Device* device, ID3D11DeviceContext* deviceContext);
	static void ReleaseAssets();

	static Mesh* sphere;
	static Mesh* quad;
	static Mesh* cube;
	static Mesh* player;
	static Mesh* floor;
	
	static Material* playerMat;
	static ScrollingMaterial* floorMat;
	static Material* obstacleMat;
	static Material* uiMat;
	static Material* backgroundMat;

	static VSConstantBufferLayout vsData;

private:

	static ifstream in_Stream;
	static ID3D11SamplerState* samplerState;
	static ID3D11ShaderResourceView* playerTex;
	static ID3D11ShaderResourceView* floorTex;
	static ID3D11ShaderResourceView* obstacleTex;
	static ID3D11ShaderResourceView* uiTex;
	static ID3D11ShaderResourceView* backgroundTex;

	static ID3D11VertexShader* playerVertexShader;
	static ID3D11PixelShader* playerPixelShader;
	static ID3D11VertexShader* obstacleVertexShader;
	static ID3D11PixelShader* obstaclePixelShader;
	static ID3D11VertexShader* floorVertexShader;
	static ID3D11PixelShader* floorPixelShader;
	static ID3D11VertexShader* uiVertexShader;
	static ID3D11PixelShader* uiPixelShader;
	static ID3D11VertexShader* backgroundVertexShader;
	static ID3D11PixelShader* backgroundPixelShader;

	static ID3D11InputLayout* playerShaderInputLayout;
	static ID3D11InputLayout* obstacleShaderInputLayout;
	static ID3D11InputLayout* floorShaderInputLayout;
	static ID3D11InputLayout* uiShaderInputLayout;
	static ID3D11InputLayout* backgroundShaderInputLayout;
	
	static ID3D11Buffer* vsConstantBuffer;

	static void LoadShaderPair(ID3D11VertexShader* &vertexShader, LPCWSTR vertexShaderPath, ID3D11PixelShader* &pixelShader, LPCWSTR pixelShaderPath, ID3D11InputLayout* &inputLayout, ID3D11Device* device);
	static void CreateInputLayoutDescFromVertexShaderSignature(ID3DBlob* shaderBlob, ID3D11InputLayout** inputLayout, ID3D11Device* device);
	static void LoadMesh(std::vector<XMFLOAT3> &positions, std::vector<std::array<UINT, 3>> &indices, std::vector<XMFLOAT2> &vertTexCoord, std::vector<XMFLOAT3> &norms);
	//static Material* LoadMaterial(const char* filePath, ID3D11ShaderResourceView* textureView, ID3D11SamplerState* samplerState, ID3D11VertexShader* vertexShader, );
};

