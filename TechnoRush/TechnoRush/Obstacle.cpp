#include "Obstacle.h"

Obstacle::Obstacle(float w, std::vector<Vertex>& vertices, std::vector<UINT>& indices,
	ID3D11Device *device, VSConstantBufferLayout *constantBufferLayout, Material *material): 
	GameEntity(vertices, indices, device, constantBufferLayout, material)
{
	float width = w;
}


Obstacle::~Obstacle()
{
}
