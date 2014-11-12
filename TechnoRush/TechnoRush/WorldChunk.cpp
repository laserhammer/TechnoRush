#include "WorldChunk.h"


WorldChunk::WorldChunk()
{
	position = XMFLOAT4(0.0f, 0.0f, 0.0f, 1.0f);
	obstPos = XMFLOAT4(0.0f, 0.0f, 0.0f, 1.0f);
	Obst = std::vector <GameEntity*>();
}

WorldChunk::WorldChunk(float xpos, float ypos, float zpos)
{
	position = XMFLOAT4(xpos, ypos, zpos, 1.0f);
	Obst = std::vector <GameEntity*>();
	generate(position);
}


WorldChunk::~WorldChunk()
{
}

void WorldChunk::update(XMFLOAT4 vel, float dt)
{
	

	position.x += vel.x;
	position.y += vel.y;
	position.z += vel.z;
	for (int i = 0; i < Obst.size(); i++)
	{
		obstPos = Obst[i]->position();
		obstPos.x += vel.x;
		obstPos.y += vel.y;
		obstPos.z += vel.z;
		Obst[i]->position(obstPos);
		Obst[i]->Update(dt);
	}

	if(position.z < - 5)
	{
		position.z += 20;
		generate(position);
	}
}

void WorldChunk::generate(XMFLOAT4 pos)
{
	for each(GameEntity* ob in Obst)
	{
		ob->position(XMFLOAT4(pos.x + (rand() % 10 - 5), 0.0f, pos.z + (rand() % 10 - 5), 1.0f));
	}
}

XMFLOAT4 WorldChunk::getPosition()
{
	return position;
}
