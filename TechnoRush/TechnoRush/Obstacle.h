#pragma once
#include "GameEntity.h"


class Obstacle : GameEntity
{
public:
	Obstacle(float w, std::vector<Vertex>& vertices, 
		std::vector<UINT>& indices,
		ID3D11Device *device,
		VSConstantBufferLayout *constantBufferLayout,
		Material *material);
	~Obstacle();

private:
	float width;
};

