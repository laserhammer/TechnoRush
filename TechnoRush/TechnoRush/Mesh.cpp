#include "Mesh.h"
#include "Game.h"


Mesh::Mesh(Vertex *vertices, UINT verticies_Length, UINT *indices, UINT indicies_Length, ID3D11Device *device)
{
	GenerateBuffers(vertices, verticies_Length, indices, indicies_Length, device);
	num_indicies = indicies_Length;
}


Mesh::~Mesh()
{
	ReleaseMacro(vertexBuffer);
	ReleaseMacro(indexBuffer);
}

ID3D11Buffer* Mesh::VertexBuffer(){ return vertexBuffer; }
ID3D11Buffer* Mesh::IndexBuffer(){ return indexBuffer; }
UINT Mesh::Num_Indicies(){ return num_indicies; }

void Mesh::GenerateBuffers(Vertex *vertices, UINT verticies_Length, UINT *indices, UINT indicies_Length, ID3D11Device *device)
{
	// Create the vertex buffer
	D3D11_BUFFER_DESC vbd;
	vbd.Usage = D3D11_USAGE_IMMUTABLE;
	vbd.ByteWidth = sizeof(Vertex) * verticies_Length; 
	vbd.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	vbd.CPUAccessFlags = 0;
	vbd.MiscFlags = 0;
	vbd.StructureByteStride = 0;
	D3D11_SUBRESOURCE_DATA initialVertexData;
	initialVertexData.pSysMem = vertices;
	HR(device->CreateBuffer(&vbd, &initialVertexData, &vertexBuffer));

	// Create the index buffer
	D3D11_BUFFER_DESC ibd;
	ibd.Usage = D3D11_USAGE_IMMUTABLE;
	ibd.ByteWidth = sizeof(UINT) * indicies_Length;
	ibd.BindFlags = D3D11_BIND_INDEX_BUFFER;
	ibd.CPUAccessFlags = 0;
	ibd.MiscFlags = 0;
	ibd.StructureByteStride = 0;
	D3D11_SUBRESOURCE_DATA initialIndexData;
	initialIndexData.pSysMem = indices;
	HR(device->CreateBuffer(&ibd, &initialIndexData, &indexBuffer));
}
