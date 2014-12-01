#include "WorldManager.h"
#include "InputManager.h"


WorldManager::WorldManager()
{
	entities = std::vector <GameEntity*>() ;
	velocity = XMFLOAT4(0.0f, 0.0f, 0.0f, 0.0f);
	slowVel = XMFLOAT4(0.0f, 0.0f, 0.0f, 0.0f);
	accel = XMFLOAT4(0.0f, 0.0f, 0.0f, 0.0f);
	worldChunks[0] = new WorldChunk(0.0f, 0.0f, 40.0f);
	worldChunks[1] = new WorldChunk(40.0f, 0.0f, 40.0f);
	worldChunks[2] = new WorldChunk(-40.0f, 0.0f, 40.0f);
	worldChunks[3] = new WorldChunk(0.0f, 0.0f, 80.0);
	worldChunks[4] = new WorldChunk(40.0f, 0.0f, 80.0f);
	worldChunks[5] = new WorldChunk(-40.0f, 0.0f, 80.0f);
	worldChunks[6] = new WorldChunk(0.0f, 0.0f, 120.0);
	worldChunks[7] = new WorldChunk(40.0f, 0.0f, 120.0f);
	worldChunks[8] = new WorldChunk(-40.0f, 0.0f, 120.0f);

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
	//use input to set acceleration
	if (InputManager::rArrowKey)
	{
		accel.x += -60.0*dt;
	}
	else if (InputManager::lArrowKey)
	{
		accel.x += 60.0*dt;
	}
	
	accel.z += -40.0f*dt;

	if (InputManager::uArrowKey)
	{
		accel.z -= 40.0f*dt;
	}
	else if (InputManager::dArrowKey)
	{
		accel.z += 40.0f*dt;
	}

	//give the player a speed boost if they are flying close to the obstacles
	for (int i = 0; i < entities.size(); i++)
	{
		if (entities[i]->position().z - 1.25f < 2.0f && entities[i]->position().z + 1.25f > -2.0f)
		{
			if (entities[i]->position().x - 1.25f < 3.0f && entities[i]->position().x + 1.25f > -3.0f)
			{
				accel.z -=  sqrt((entities[i]->position().x * entities[i]->position().x) + (entities[i]->position().z * entities[i]->position().z))*0.08;
			}
		}
	}

	//limit max acceleration
	if (accel.z < -80)
	{
		accel.z = -80;
	}

	//add acceleration to velocity
	velocity.x += accel.x;
	velocity.z += accel.z;

	slowVel.x = velocity.x;
	slowVel.z = velocity.z / 3;

	//'move' the world
	if (InputManager::dArrowKey)
	{
		//slow everything down if the player is holding the down key
		for (int i = 0; i < 9; i++)
		{
			worldChunks[i]->update(slowVel, dt);
		}
	}
	else
	{
		for (int i = 0; i < 9; i++)
		{
			worldChunks[i]->update(velocity, dt);
		}
		velocity.z *= 0.999f;
	}
	
	//reset acceleration
	velocity.x *= 0.995f;
	accel.z = 0;
	accel.x = 0;

	checkCollision();
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

void WorldManager::checkCollision()
{
	//check to see if any obstacle has collided with the player
	for (int i = 0; i < entities.size(); i++)
	{
		if (entities[i]->position().z - 1.25f < -1.75f && entities[i]->position().z + 1.25f > -2.0f)
		{
			if (entities[i]->position().x - 1.25f < 0.5f && entities[i]->position().x + 1.25f > -0.5f)
			{
				collide = true;
			}
		}
	}
}

//return worldchunks to their starting positions and randomize obstacle locations
void WorldManager::resetWorld()
{
	velocity.x = 0;
	velocity.z = 0;

	worldChunks[0]->setPosition(XMFLOAT4(0.0f, 0.0f, 40.0f, 1.0f));
	worldChunks[1]->setPosition(XMFLOAT4(40.0f, 0.0f, 40.0f, 1.0f));
	worldChunks[2]->setPosition(XMFLOAT4(-40.0f, 0.0f, 40.0f, 1.0f));
	worldChunks[3]->setPosition(XMFLOAT4(0.0f, 0.0f, 80.0, 1.0f));
	worldChunks[4]->setPosition(XMFLOAT4(40.0f, 0.0f, 80.0f, 1.0f));
	worldChunks[5]->setPosition(XMFLOAT4(-40.0f, 0.0f, 80.0f, 1.0f));
	worldChunks[6]->setPosition(XMFLOAT4(0.0f, 0.0f, 120.0, 1.0f));
	worldChunks[7]->setPosition(XMFLOAT4(40.0f, 0.0f, 120.0f, 1.0f));
	worldChunks[8]->setPosition(XMFLOAT4(-40.0f, 0.0f, 120.0f, 1.0f));

	for each(WorldChunk* chunk in worldChunks)
	{
		chunk->generate(chunk->getPosition());
	}
}

bool WorldManager::getCollide()
{
	return collide;
}

void WorldManager::setCollide(bool col)
{
	collide = col;
}

float WorldManager::getSpeed()
{
	return velocity.z;
}