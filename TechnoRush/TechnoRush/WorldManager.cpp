#include "WorldManager.h"
#include "InputManager.h"
#include "ScrollingMaterial.h"


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
		accel.x += -0.12f*dt;
	}
	else if (InputManager::lArrowKey)
	{
		accel.x += 0.12f*dt;
	}
	
	accel.z += -0.08f*dt;

	if (InputManager::uArrowKey)
	{
		accel.z -= 0.08f*dt;
	}
	else if (InputManager::dArrowKey)
	{
		accel.z += 0.08f*dt;
	}

	//give the player a speed boost if they are flying close to the obstacles
	unsigned int size = entities.size();
	for (unsigned int i = 0; i < size; i++)
	{
		if (entities[i]->position().z - 1.25f < 2.0f && entities[i]->position().z + 1.25f > -2.0f)
		{
			if (entities[i]->position().x - 1.25f < 3.0f && entities[i]->position().x + 1.25f > -3.0f)
			{
				accel.z -=  sqrt((entities[i]->position().x * entities[i]->position().x) + (entities[i]->position().z * entities[i]->position().z))*0.0002f;
			}
		}
	}

	//limit max acceleration
	if (accel.z < -0.16f)
	{
		accel.z = -0.16f;
	}

	//add acceleration to velocity
	velocity.x += accel.x;
	velocity.z += accel.z;

	slowVel.x = velocity.x;
	slowVel.z = velocity.z / 3;

	//Scroll the floor
	float scrollWrap = 25.0f;
	float velocityScale = 0.25f;

	//'move' the world
	if (InputManager::dArrowKey)
	{
		//slow everything down if the player is holding the down key
		for (int i = 0; i < 9; i++)
		{
			worldChunks[i]->update(slowVel, dt);
		}
		_scroll = XMFLOAT2(modff(-slowVel.x   * velocityScale + _scroll.x, &scrollWrap), modff(slowVel.z  * velocityScale + _scroll.y, &scrollWrap));
	}
	else
	{
		for (int i = 0; i < 9; i++)
		{
			worldChunks[i]->update(velocity, dt);
			
		}
		_scroll = XMFLOAT2(modff(-velocity.x   * velocityScale + _scroll.x, &scrollWrap), modff(velocity.z  * velocityScale + _scroll.y, &scrollWrap));
	}
	

	((ScrollingMaterial*)(_floor->mat()))->SetScroll(_scroll);
	
	//reset acceleration
	velocity.z *= 0.997f;
	velocity.x *= 0.995f;
	accel.z = 0;
	accel.x = 0;

	checkCollision();

	//Scroll the floor
	//float scrollWrap = 50.0f;
	//float velocityScale = 1.0f;
	//_scroll = XMFLOAT2(modff(velocity.x * dt * velocityScale + _scroll.x, &scrollWrap), modff(velocity.z * dt * velocityScale + _scroll.y, &scrollWrap));

	//((ScrollingMaterial*)(_floor->mat()))->SetScroll(_scroll);
}

void WorldManager::getEntities(std::vector<GameEntity*>* _entities)
{
	entities = *_entities;
	unsigned int size = entities.size();
	for (unsigned int i = 0; i < size; i++)
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
	unsigned int size = entities.size();
	for (unsigned int i = 0; i < size; i++)
	{
		if (entities[i]->position().z - 1.25f < -1.75f && entities[i]->position().z + 1.25f > -2.0f)
		{
			if (entities[i]->position().x - 0.9f < 0.5f && entities[i]->position().x + 0.9f > -0.5f)
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
void WorldManager::SetFloor(GameEntity* floor)
{
	_floor = floor;
}