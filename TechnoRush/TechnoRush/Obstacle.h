#pragma once
#include "GameEntity.h"


class Obstacle : GameEntity
{
public:
	Obstacle(float w, Vertex *vertices, UINT verticies_Length,
		UINT *indices, UINT indicies_Length,
		ID3D11Device *device,
		VSConstantBufferLayout *constantBufferLayout,
		Material *material);
	~Obstacle();

private:
	float width;
};

