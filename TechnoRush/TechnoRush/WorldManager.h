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
	bool getCollide();
	void setCollide(bool col);
	float getSpeed();
	void resetWorld();

private:
	bool _forward, _back;
	bool _left, _right;
	bool collide;
	XMFLOAT4 velocity;
	XMFLOAT4 slowVel;
	XMFLOAT4 accel;
	WorldChunk* worldChunks[9];
	//player
	std::vector<GameEntity*> entities;
	
	void checkCollision();

};

