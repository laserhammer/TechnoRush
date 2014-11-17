#pragma once

#include <DirectXMath.h>
#include <vector>
#include <array>
#include "DirectXGame.h"

#if defined(DEBUG) || defined(_DEBUG)
#define _CRTDBG_MAP_ALLOC
#include <crtdbg.h>
#endif

using namespace DirectX;

struct Vertex
{
	XMFLOAT3 Position;
	XMFLOAT2 UV;
	XMFLOAT3 Normal;
};

class Mesh
{
public:
	Mesh(std::vector<XMFLOAT3>* positions, std::vector<std::array<UINT, 3>>* indices, std::vector<XMFLOAT2>* uvs, std::vector<XMFLOAT3>* norms, ID3D11Device *device);
	~Mesh();

	ID3D11Buffer* VertexBuffer();
	//ID3D11Buffer* VertexIndexBuffer();
	ID3D11Buffer* UvBuffer();
	//ID3D11Buffer* UvIndexBuffer;
	ID3D11Buffer* NormalBuffer();
	//ID3D10Buffer* NormalIndexBuffer;
	UINT NumIndices();

private:

	void GenerateBuffers(std::vector<XMFLOAT3>* positions, std::vector<std::array<UINT, 3>>* indices, std::vector<XMFLOAT2>* uvs, std::vector<XMFLOAT3>* norms, ID3D11Device *device);

private:

	//Member Variables
	ID3D11Buffer* _vertexBuffer;
	//ID3D11Buffer* _vertIndexBuffer;
	ID3D11Buffer* _uvBuffer;
	//ID3D11Buffer* _uvIndexBuffer;
	ID3D11Buffer* _normalBuffer;
	//ID3D10Buffer* _normalIndexBuffer;
	UINT _numIndices;

};

