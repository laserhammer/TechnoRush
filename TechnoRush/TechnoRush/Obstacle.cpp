#include "Obstacle.h"

Obstacle::Obstacle(float w, std::vector<XMFLOAT3>* positions, std::vector<std::array<UINT, 3>>* indices, std::vector<XMFLOAT2>* uvs, std::vector<XMFLOAT3>* norms, XMFLOAT4* color,
	ID3D11Device *device, VSConstantBufferLayout *constantBufferLayout, Material *material): 
	GameEntity(positions, indices, uvs, norms, color, device, constantBufferLayout, material)
{
	float width = w;
}


Obstacle::~Obstacle()
{
}
