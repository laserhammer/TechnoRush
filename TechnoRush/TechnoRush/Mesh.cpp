
#include "Mesh.h"
#include "Game.h"



Mesh::Mesh(std::vector<XMFLOAT3>* positions, std::vector<std::array<UINT, 3>>* indices, std::vector<XMFLOAT2>* uvs, std::vector<XMFLOAT3>* norms, XMFLOAT4* color, ID3D11Device *device)
{
	GenerateBuffers(positions, indices, uvs, norms, color, device);
	_numIndices = indices->size();
}


Mesh::~Mesh()
{
	ReleaseMacro(_vertexBuffer);
	//ReleaseMacro(_vertIndexBuffer);
	ReleaseMacro(_vertexBuffer);
	//ReleaseMacro(_vertIndexBuffer);
	ReleaseMacro(_vertexBuffer);
	//ReleaseMacro(_vertIndexBuffer);
}

ID3D11Buffer* Mesh::VertexBuffer() { return _vertexBuffer; }
//ID3D11Buffer* VertexIndexBuffer();
ID3D11Buffer* Mesh::UvBuffer() { return _uvBuffer; }
//ID3D11Buffer* UvIndexBuffer;
ID3D11Buffer* Mesh::NormalBuffer() { return _normalBuffer; }
//ID3D10Buffer* NormalIndexBuffer;
UINT Mesh::NumIndices() { return _numIndices; }

void Mesh::GenerateBuffers(std::vector<XMFLOAT3>* positions, std::vector<std::array<UINT, 3>>* indices, std::vector<XMFLOAT2>* uvs, std::vector<XMFLOAT3>* norms, XMFLOAT4* color, ID3D11Device *device)
{
	_numIndices = indices->size();
	std::vector<Vertex> vertices = std::vector<Vertex>();
	vertices.resize(_numIndices);
	for (int i = 0; i < _numIndices; ++i)
	{
		//Vertex vert;
		//vert.Position = positions->at((*indices)[i][0] - 1);
		//vert.UV = uvs->at((*indices)[i][1] - 1);
		//vert.Normal = norms->at((*indices)[i][2] - 1);
		//vert.Color = *color;
		vertices.push_back(Vertex());
		vertices[i].Position = positions->at((*indices)[i][0] - 1);
		vertices[i].UV = uvs->at((*indices)[i][1] - 1);
		vertices[i].Normal = norms->at((*indices)[i][2] - 1);
		vertices[i].Color = *color;
	}
	// Create the vertex buffer
	D3D11_BUFFER_DESC vbd;
	vbd.Usage = D3D11_USAGE_IMMUTABLE;
	vbd.ByteWidth = sizeof(Vertex) * _numIndices;
	vbd.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	vbd.CPUAccessFlags = 0;
	vbd.MiscFlags = 0;
	vbd.StructureByteStride = 0;
	D3D11_SUBRESOURCE_DATA initialVertexData;
	initialVertexData.pSysMem = &vertices[0];
	HR(device->CreateBuffer(&vbd, &initialVertexData, &_vertexBuffer));

	// Create the index buffer
	/*
	D3D11_BUFFER_DESC ibd;
	ibd.Usage = D3D11_USAGE_IMMUTABLE;
	ibd.ByteWidth = sizeof(UINT) * indices->size();
	ibd.BindFlags = D3D11_BIND_INDEX_BUFFER;
	ibd.CPUAccessFlags = 0;
	ibd.MiscFlags = 0;
	ibd.StructureByteStride = 0;
	D3D11_SUBRESOURCE_DATA initialIndexData;
	initialIndexData.pSysMem = &indices[0][0];
	HR(device->CreateBuffer(&ibd, &initialIndexData, &indexBuffer));
	*/
}
