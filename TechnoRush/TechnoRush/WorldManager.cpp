#include "WorldManager.h"


WorldManager::WorldManager()
{
	entities = std::vector <GameEntity*>() ;
	velocity = XMFLOAT4(0.0f, 0.0f, 0.0f, 0.0f);
	worldChunks[0] = new WorldChunk(0.0f, 0.0f, 0.0f);
	worldChunks[1] = new WorldChunk(10.0f, 0.0f, 0.0f);
	worldChunks[2] = new WorldChunk(-10.0f, 0.0f, 0.0f);
	worldChunks[3] = new WorldChunk(0.0f, 0.0f, 10.0);
	worldChunks[4] = new WorldChunk(10.0f, 0.0f, 10.0f);
	worldChunks[5] = new WorldChunk(-10.0f, 0.0f, 10.0f);

}


WorldManager::~WorldManager()
{
}

void WorldManager::Update(float dt)
{
	//TODO: get velocity from player
	velocity.z = -0.005f;

	for (int i = 0; i < 6; i++)
	{
		worldChunks[i]->update(velocity, dt);
	}
}

void WorldManager::getEntities(std::vector<GameEntity*>* _entities)
{
	entities = *_entities;

	for (int i = 0; i < entities.size(); i++)
	{
		worldChunks[i % 6]->Obst.push_back(entities[i]);
	}

	for each(WorldChunk* chunk in worldChunks)
	{
		chunk->generate(chunk->getPosition());
	}
}
