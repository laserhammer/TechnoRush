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
	float dx = vel.x * dt;
	float dy = vel.y * dt;
	float dz = vel.z * dt;
	position.x += dx;
	position.y += dy;
	position.z += dz;
	for (int i = 0; i < Obst.size(); i++)
	{
		obstPos = Obst[i]->position();
		obstPos.x += dx;
		obstPos.y += dy;
		obstPos.z += dz;
		Obst[i]->position(obstPos);
		Obst[i]->Update(dt);
	}

	if(position.z < - 20)
	{
		position.z += 60;
		generate(position);
	}

	if (position.x > 30)
	{
		position.x -= 60;
		generate(position);
	}
	else if (position.x < -30)
	{
		position.x += 60;
		generate(position);
	}
}

void WorldChunk::generate(XMFLOAT4 pos)
{
	for each(GameEntity* ob in Obst)
	{
		ob->position(XMFLOAT4(pos.x + (rand() % 20 - 10), 0.0f, pos.z + (rand() % 20 - 10), 1.0f));
	}
}

XMFLOAT4 WorldChunk::getPosition()
{
	return position;
}
