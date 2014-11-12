#pragma once
#include <DirectXMath.h>
#include "Obstacle.h"
#include <vector>

using namespace DirectX;

class WorldChunk
{
public:
	std::vector<GameEntity*> Obst;

	WorldChunk();
	WorldChunk(float xpos, float ypos, float zpos);
	~WorldChunk();
	void update(XMFLOAT4 Velocity, float dt);
	void generate(XMFLOAT4 pos);
	XMFLOAT4 getPosition();
private:
	XMFLOAT4 position;
	XMFLOAT4 obstPos;

	
	



};

