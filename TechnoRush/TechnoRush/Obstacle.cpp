#include "Obstacle.h"

Obstacle::Obstacle(float w, Vertex *vertices, UINT verticies_Length, UINT *indices, UINT indicies_Length,
	ID3D11Device *device, VSConstantBufferLayout *constantBufferLayout, Material *material): 
	GameEntity(vertices, verticies_Length, indices, indicies_Length, device, constantBufferLayout, material)
{
	float width = w;
}


Obstacle::~Obstacle()
{
}
