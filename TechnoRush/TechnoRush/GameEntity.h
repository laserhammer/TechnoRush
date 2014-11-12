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
	GameEntity(std::vector<Vertex>& vertices,
		std::vector<UINT>& indices,
		ID3D11Device *device,
		VSConstantBufferLayout *constantBufferLayout,
		Material *material);
	~GameEntity();

	//XMFLOAT4X4* World();
	XMFLOAT4 position();
	void position(XMFLOAT4 newPosition);
	unsigned int layer();
	void layer(int newLayer);

	void Update(float dt);
	void Draw(ID3D11DeviceContext* deviceContext);

private:
	void init();

private:

	// Member variables
	Mesh* _mesh;
	XMFLOAT4X4 _world;
	XMFLOAT4 _position;
	XMFLOAT4 _rotation;
	XMFLOAT4 _scale;
	//float velocity[2];
	VSConstantBufferLayout *_constantBufferLayout;

	Material* _material;

	unsigned int _layer;
};

