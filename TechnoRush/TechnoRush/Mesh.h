#pragma once

#include <DirectXMath.h>
#include <vector>
#include "DirectXGame.h"

#if defined(DEBUG) || defined(_DEBUG)
#define _CRTDBG_MAP_ALLOC
#include <crtdbg.h>
#endif

using namespace DirectX;

struct Vertex
{
	XMFLOAT3 Position;
	XMFLOAT4 Color;
	XMFLOAT2 UV;
};

class Mesh
{
public:
	Mesh(std::vector<Vertex>& vertices, std::vector<UINT>& indices, ID3D11Device *device);
	~Mesh();

	ID3D11Buffer* VertexBuffer();
	ID3D11Buffer* IndexBuffer();
	UINT Num_Indicies();

private:

	void GenerateBuffers(std::vector<Vertex>& vertices, std::vector<UINT>& indices, ID3D11Device *device);

private:

	//Member Variables
	ID3D11Buffer* vertexBuffer;
	ID3D11Buffer* indexBuffer;
	UINT num_indicies;

};

