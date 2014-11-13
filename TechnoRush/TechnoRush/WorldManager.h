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
	bool _forward, _back;
	bool _left, _right;
	XMFLOAT4 velocity;
	WorldChunk* worldChunks[9];
	//player
	std::vector<GameEntity*> entities;

};

