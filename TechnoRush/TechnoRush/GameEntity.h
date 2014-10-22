#pragma once

#include <DirectXMath.h>
#include "DirectXGame.h"
#include "Mesh.h"
#include "Material.h"

#if defined(DEBUG) || defined(_DEBUG)
#define _CRTDBG_MAP_ALLOC
#include <crtdbg.h>
#endif

using namespace DirectX;

struct VSConstantBufferLayout;

class GameEntity
{
public:
	GameEntity(	Vertex *vertices,				UINT verticies_Length,			
				UINT *indices,					UINT indicies_Length,			
				ID3D11Device *device,			ID3D11Buffer *vsConstantBuffer,	
				VSConstantBufferLayout *constantBufferLayout, 
				Material *material);
	~GameEntity();

	XMFLOAT4X4* World();

	
	void Update(float dt);
	void Draw(ID3D11DeviceContext* deviceContext);

private:
	void init();

private:

	// Member variables
	Mesh* mesh;
	XMFLOAT4X4 world;
	XMFLOAT4 position;
	XMFLOAT4 rotation;
	XMFLOAT4 scale;
	float velocity[2];
	ID3D11Buffer* vsConstantBuffer;
	VSConstantBufferLayout *constantBufferLayout;

	Material* material;
};
