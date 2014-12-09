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
	unsigned int size = Obst.size();
	for (unsigned int i = 0; i < size; i++)
	{
		obstPos = Obst[i]->position();
		obstPos.x += vel.x;
		obstPos.y += vel.y;
		obstPos.z += vel.z;
		
		//Cover up the obstacle popping in
		if (obstPos.z > 60.0f)
		{
			obstPos.y = 7.5f - 0.125f * obstPos.z;
		}
		else
		{
			obstPos.y = 0.0f;
		}

		Obst[i]->position(obstPos);
		Obst[i]->Update(dt);
	}

	if(position.z < - 40)
	{
		position.z += 120;
		generate(position);
	}

	if (position.x > 60)
	{
		position.x -= 120;
		generate(position);
	}
	else if (position.x < -60)
	{
		position.x += 120;
		generate(position);
	}
}

void WorldChunk::generate(XMFLOAT4 pos)
{
	for each(GameEntity* ob in Obst)
	{
		ob->position(XMFLOAT4(pos.x + (rand() % 40 - 10), 0.0f, pos.z + (rand() % 40 - 10), 1.0f));
	}
}

XMFLOAT4 WorldChunk::getPosition()
{
	return position;
}

void WorldChunk::setPosition(XMFLOAT4 pos)
{
	position = pos;
}
