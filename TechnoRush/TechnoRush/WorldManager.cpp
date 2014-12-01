#include "WorldManager.h"
#include "InputManager.h"
#include "ScrollingMaterial.h"


WorldManager::WorldManager()
{
	entities = std::vector <GameEntity*>() ;
	velocity = XMFLOAT4(0.0f, 0.0f, 0.0f, 0.0f);
	worldChunks[0] = new WorldChunk(0.0f, 0.0f, 0.0f);
	worldChunks[1] = new WorldChunk(20.0f, 0.0f, 0.0f);
	worldChunks[2] = new WorldChunk(-20.0f, 0.0f, 0.0f);
	worldChunks[3] = new WorldChunk(0.0f, 0.0f, 20.0);
	worldChunks[4] = new WorldChunk(20.0f, 0.0f, 20.0f);
	worldChunks[5] = new WorldChunk(-20.0f, 0.0f, 20.0f);
	worldChunks[6] = new WorldChunk(0.0f, 0.0f, 40.0);
	worldChunks[7] = new WorldChunk(20.0f, 0.0f, 40.0f);
	worldChunks[8] = new WorldChunk(-20.0f, 0.0f, 40.0f);

}


WorldManager::~WorldManager()
{
	for (int i = 0; i < 9; ++i)
	{
		delete worldChunks[i];
	}
}

void WorldManager::Update(float dt)
{
	if (InputManager::rArrowKey)
	{
		velocity.x = -5.0;
	}
	else if (InputManager::lArrowKey)
	{
		velocity.x = 5.0;
	}
	else
	{
		velocity.x = 0;
	}
	//TODO: get velocity from player
	velocity.z = -7.5f;

	if (InputManager::uArrowKey)
	{
		velocity.z -= 7.5;
	}
	else if (InputManager::dArrowKey)
	{
		velocity.z += 6.0;
	}

	for (int i = 0; i < 9; i++)
	{
		worldChunks[i]->update(velocity, dt);
	}
	float scrollWrap = 50.0f;
	float velocityScale = 0.5f;
	_scroll = XMFLOAT2(modff(-velocity.x * dt  * velocityScale + _scroll.x, &scrollWrap), modff(velocity.z * dt * velocityScale + _scroll.y, &scrollWrap));

	((ScrollingMaterial*)(_floor->mat()))->SetScroll(_scroll);
}

void WorldManager::getEntities(std::vector<GameEntity*>* _entities)
{
	entities = *_entities;

	for (int i = 0; i < entities.size(); i++)
	{
		worldChunks[i % 9]->Obst.push_back(entities[i]);
	}

	for each(WorldChunk* chunk in worldChunks)
	{
		chunk->generate(chunk->getPosition());
	}
}

void WorldManager::SetFloor(GameEntity* floor)
{
	_floor = floor;
}