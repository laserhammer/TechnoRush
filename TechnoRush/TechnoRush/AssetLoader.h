#pragma once

#include "GameEntity.h"
#include "Game.h"
#include "Material.h"
#include "DirectXGame.h"


using namespace std;

static class AssetLoader
{
public:


	// This will load OBJs
	static GameEntity* LoadOBJ(ID3D11Device *device, VSConstantBufferLayout *constantBufferLayout, Material* material, ifstream& in_Stream);




	
};

