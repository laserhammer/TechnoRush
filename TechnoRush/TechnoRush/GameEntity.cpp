
#include "GameEntity.h"
#include "Mesh.h"
#include "Game.h"
#include "Material.h"



GameEntity::GameEntity(Vertex *vertices, UINT verticies_Length, UINT *indices, UINT indicies_Length, ID3D11Device *device, VSConstantBufferLayout *constantBufferLayout, Material* material)
{
	mesh = new Mesh(vertices, verticies_Length, indices, indicies_Length, device);
	this->constantBufferLayout = constantBufferLayout;
	this->material = material;
	init();
}


GameEntity::~GameEntity()
{
	delete mesh;
}


void GameEntity::init()
{
	scale = XMFLOAT4(1.0f, 1.0f, 1.0f, 0.0f);
	XMStoreFloat4(&rotation, XMQuaternionIdentity());
	position = XMFLOAT4(0.0f, 0.0f, 0.0f, 1.0f);
}

void GameEntity::Update(float dt)
{
	// Load

	XMVECTOR pos = XMLoadFloat4(&position);
	//Rotation
	XMVECTOR rot = XMLoadFloat4(&rotation);
	// Scale
	XMVECTOR scal = XMLoadFloat4(&scale);

	// Change

	// Update world matrix
	XMMATRIX worldMatrix = XMMatrixTranspose(XMMatrixTransformation(pos, rot, scal, pos, rot, pos));

	// Store 

	XMStoreFloat4(&position, pos);
	XMStoreFloat4(&rotation, rot);
	XMStoreFloat4(&scale, scal);
	XMStoreFloat4x4(&world, worldMatrix);
}

void GameEntity::Draw(ID3D11DeviceContext* deviceContext)
{
	// Set up the input assembler
	deviceContext->IASetInputLayout(material->inputLayout());
	deviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

	// Set the current vertex and pixel shaders, as well the constant buffer for the vert shader
	deviceContext->VSSetShader(material->vertexShader(), NULL, 0);
	ID3D11Buffer* buffer = material->vsConstantBuffer();
	deviceContext->VSSetConstantBuffers(
		0,	// Corresponds to the constant buffer's register in the vertex shader
		1,
		&(buffer));
	deviceContext->PSSetShader(material->pixelShader(), NULL, 0);

	// Set draw buffer
	UINT stride = sizeof(Vertex);
	UINT offset = 0;
	ID3D11Buffer *vertBuffer = mesh->VertexBuffer();
	ID3D11Buffer *indexBuffer = mesh->IndexBuffer();
	deviceContext->IASetVertexBuffers(0, 1, &vertBuffer, &stride, &offset);
	deviceContext->IASetIndexBuffer(indexBuffer, DXGI_FORMAT_R32_UINT, 0);

	// Set material resources
	ID3D11ShaderResourceView* textureView = material->textureView();
	ID3D11SamplerState* samplerState = material->samplerState();
	deviceContext->PSSetShaderResources(0, 1, &textureView);
	deviceContext->PSSetSamplers(0, 1, &samplerState);

	// Send constant buffer
	constantBufferLayout->world = world;
	deviceContext->UpdateSubresource(material->vsConstantBuffer(), 0, NULL, constantBufferLayout, 0, 0);

	deviceContext->DrawIndexed(mesh->Num_Indicies(), 0, 0);
}
