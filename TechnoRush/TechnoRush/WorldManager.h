#pragma once
#include <d3d11.h>
#include <DirectXMath.h>
#include "WorldChunk.h"
#include <vector>

using namespace DirectX;

class WorldManager
{
public:
	WorldManager();
	~WorldManager();

	void Update(float dt);
	void getEntities(std::vector<GameEntity*>* _entities);

private:
	XMFLOAT4 velocity;
	WorldChunk* worldChunks[6];
	//player
	std::vector<GameEntity*> entities;
};

