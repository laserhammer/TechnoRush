
#include "Mesh.h"
#include "Game.h"



Mesh::Mesh(std::vector<Vertex>& vertices, std::vector<UINT>& indices, ID3D11Device *device)
{
	GenerateBuffers(vertices, indices, device);
	num_indicies = indices.size();
}


Mesh::~Mesh()
{
	ReleaseMacro(vertexBuffer);
	ReleaseMacro(indexBuffer);
}

ID3D11Buffer* Mesh::VertexBuffer(){ return vertexBuffer; }
ID3D11Buffer* Mesh::IndexBuffer(){ return indexBuffer; }
UINT Mesh::Num_Indicies(){ return num_indicies; }

void Mesh::GenerateBuffers(std::vector<Vertex>& vertices, std::vector<UINT>& indices, ID3D11Device *device)
{
	// Create the vertex buffer
	D3D11_BUFFER_DESC vbd;
	vbd.Usage = D3D11_USAGE_IMMUTABLE;
	vbd.ByteWidth = sizeof(Vertex) * vertices.size();
	vbd.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	vbd.CPUAccessFlags = 0;
	vbd.MiscFlags = 0;
	vbd.StructureByteStride = 0;
	D3D11_SUBRESOURCE_DATA initialVertexData;
	initialVertexData.pSysMem = &vertices[0];
	HR(device->CreateBuffer(&vbd, &initialVertexData, &vertexBuffer));

	// Create the index buffer
	D3D11_BUFFER_DESC ibd;
	ibd.Usage = D3D11_USAGE_IMMUTABLE;
	ibd.ByteWidth = sizeof(UINT) * indices.size();
	ibd.BindFlags = D3D11_BIND_INDEX_BUFFER;
	ibd.CPUAccessFlags = 0;
	ibd.MiscFlags = 0;
	ibd.StructureByteStride = 0;
	D3D11_SUBRESOURCE_DATA initialIndexData;
	initialIndexData.pSysMem = &indices[0];
	HR(device->CreateBuffer(&ibd, &initialIndexData, &indexBuffer));
}
