
#include "GameEntity.h"
#include "Mesh.h"
#include "Game.h"
#include "Material.h"



GameEntity::GameEntity(Vertex *vertices, UINT verticies_Length, UINT *indices, UINT indicies_Length, ID3D11Device *device, VSConstantBufferLayout *constantBufferLayout, Material* material)
{
	_mesh = new Mesh(vertices, verticies_Length, indices, indicies_Length, device);
	this->_constantBufferLayout = constantBufferLayout;
	this->_material = material;
	init();
}


GameEntity::~GameEntity()
{
	delete _mesh;
}


void GameEntity::init()
{
	_scale = XMFLOAT4(1.0f, 1.0f, 1.0f, 0.0f);
	XMStoreFloat4(&_rotation, XMQuaternionIdentity());
	_position = XMFLOAT4(0.0f, 0.0f, 0.0f, 1.0f);
	_layer = 1;
}

void GameEntity::Update(float dt)
{
	// Load

	XMVECTOR pos = XMLoadFloat4(&_position);
	//Rotation
	XMVECTOR rot = XMLoadFloat4(&_rotation);
	// Scale
	XMVECTOR scal = XMLoadFloat4(&_scale);

	// Change

	// Update world matrix
	XMMATRIX worldMatrix = XMMatrixTranspose(XMMatrixTransformation(pos, rot, scal, pos, rot, pos));

	// Store 

	XMStoreFloat4(&_position, pos);
	XMStoreFloat4(&_rotation, rot);
	XMStoreFloat4(&_scale, scal);
	XMStoreFloat4x4(&_world, worldMatrix);
}

void GameEntity::Draw(ID3D11DeviceContext* deviceContext)
{
	// Set up the input assembler
	deviceContext->IASetInputLayout(_material->inputLayout());
	deviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

	// Set the current vertex and pixel shaders, as well the constant buffer for the vert shader
	deviceContext->VSSetShader(_material->vertexShader(), NULL, 0);
	ID3D11Buffer* buffer = _material->vsConstantBuffer();
	deviceContext->VSSetConstantBuffers(
		0,	// Corresponds to the constant buffer's register in the vertex shader
		1,
		&(buffer));
	deviceContext->PSSetShader(_material->pixelShader(), NULL, 0);

	// Set draw buffer
	UINT stride = sizeof(Vertex);
	UINT offset = 0;
	ID3D11Buffer *vertBuffer = _mesh->VertexBuffer();
	ID3D11Buffer *indexBuffer = _mesh->IndexBuffer();
	deviceContext->IASetVertexBuffers(0, 1, &vertBuffer, &stride, &offset);
	deviceContext->IASetIndexBuffer(indexBuffer, DXGI_FORMAT_R32_UINT, 0);

	// Set material resources
	ID3D11ShaderResourceView* textureView = _material->textureView();
	ID3D11SamplerState* samplerState = _material->samplerState();
	deviceContext->PSSetShaderResources(0, 1, &textureView);
	deviceContext->PSSetSamplers(0, 1, &samplerState);

	// Send constant buffer
	_constantBufferLayout->world = _world;
	deviceContext->UpdateSubresource(_material->vsConstantBuffer(), 0, NULL, _constantBufferLayout, 0, 0);

	deviceContext->DrawIndexed(_mesh->Num_Indicies(), 0, 0);
}

XMFLOAT4 GameEntity::position() { return _position; }
void GameEntity::position(XMFLOAT4 newPosition) { _position = newPosition; }
unsigned int GameEntity::layer() { return _layer; }
void GameEntity::layer(int newLayer) { _layer = newLayer; }
