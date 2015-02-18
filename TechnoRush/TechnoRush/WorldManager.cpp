#include "WorldManager.h"
#include "InputManager.h"
#include "ScrollingMaterial.h"
#include <ppl.h>

#define BASE_ACCEL -1.00f
#define ACTIVE_ACCEL -1.50f
#define ACCEL_INC -0.0005f
#define ROT_RATE 2.0f
#define STRAFE_RATE 2.0f
#define DECCEL_COEFF 0.7f
#define OBST_BOOST_COEFF 0.45f
#define FWD_FRIC_COEFF 1.25f

WorldManager::WorldManager()
{
	entities = std::vector <GameEntity*>() ;
	playerRot = XMFLOAT4(0.0f, 0.0f, 0.0f, 0.0f);
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
	_totalTime = 0.0f;
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
	_totalTime += dt;
	playerRot = entities.at(entities.size() - 1)->rotation();
	//use input to set acceleration

	//rotate player model if they are moving left or right
	if (InputManager::rArrowKey)
	{
		accel.x += -STRAFE_RATE + -velocity.z * dt;
		if (playerRot.z > -0.2f)
			playerRot.z -= ROT_RATE*dt;
	}
	else if (InputManager::lArrowKey)
	{
		accel.x += STRAFE_RATE + velocity.z * dt;
		if (playerRot.z < 0.2f)
			playerRot.z += ROT_RATE*dt;
	}
	else
	{
		if (playerRot.z <= 0.01f && playerRot.z >= -0.01f)
		{
			playerRot.z = 0.0f;
		}
		else if (playerRot.z > 0.0f)
		{
			playerRot.z -= 0.5f*dt;
		}
		else if (playerRot.z < 0.0f)
		{
			playerRot.z += 0.5f*dt;
		}
	}
	
	//increase the player's speed over time
	accel.z += ACCEL_INC * _totalTime;

	if (InputManager::uArrowKey)
	{
		accel.z += ACTIVE_ACCEL;
	}
	else if (InputManager::dArrowKey)
	{
		accel.z += BASE_ACCEL;
		accel.z *= DECCEL_COEFF;
	}
	else
		accel.z += BASE_ACCEL;

	//give the player a speed boost if they are flying close to the obstacles
	unsigned int size = entities.size()-1;
	concurrency::parallel_for(size_t(0), size, [&](size_t i)
	{
		if (entities[i]->position().z - 1.25f < 2.0f && entities[i]->position().z + 1.25f > -2.0f)
		{
			if (entities[i]->position().x - 1.25f < 3.0f && entities[i]->position().x + 1.25f > -3.0f)
			{
				accel.z -= sqrt((entities[i]->position().x * entities[i]->position().x) + (entities[i]->position().z * entities[i]->position().z))*OBST_BOOST_COEFF;
			}
		}
	});


	//add acceleration to velocity
	velocity.x += accel.x;
	velocity.z += accel.z;

	//get displacement based on time
	XMFLOAT4 displacement = XMFLOAT4(velocity.x * dt, velocity.y * dt, velocity.z * dt, 0.0f);

	slowVel.x = displacement.x;
	slowVel.z = displacement.z / 3;

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
			worldChunks[i]->update(displacement, dt);
			
		}
		_scroll = XMFLOAT2(modff(-displacement.x   * velocityScale + _scroll.x, &scrollWrap), modff(displacement.z  * velocityScale + _scroll.y, &scrollWrap));
	}
	

	((ScrollingMaterial*)(_floor->mat()))->SetScroll(_scroll);
	
	//reset acceleration
	//apply friction
	velocity.z *= 1.0f - (FWD_FRIC_COEFF * dt);
	//velocity.z -= displacement.z * 10.0f;
	velocity.x *= 1.0f - (3.5f * dt);
	//velocity.x -= displacement.x;
	accel.z = 0;
	accel.x = 0;

	checkCollision();
	entities.at(entities.size() - 1)->rotation(playerRot);
}

void WorldManager::getEntities(std::vector<GameEntity*>* _entities)
{
	entities = *_entities;
	unsigned int size = entities.size()-1;
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
	unsigned int size = entities.size()-1;
	concurrency::parallel_for(size_t(0), size, [&](size_t i)
	{
		if (entities[i]->position().z - 1.25f < -1.75f && entities[i]->position().z + 1.25f > -2.0f)
		{
			if (entities[i]->position().x - 0.9f < 0.5f && entities[i]->position().x + 0.9f > -0.5f)
			{
				collide = true;
			}
		}
	});
}

//return worldchunks to their starting positions and randomize obstacle locations
void WorldManager::resetWorld()
{
	_totalTime = 0.0f;
	velocity.x = 0;
	velocity.z = 0;

	playerRot.z = 0;
	entities.at(entities.size() - 1)->rotation(playerRot);

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