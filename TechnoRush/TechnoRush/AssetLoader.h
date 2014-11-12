#pragma once
#include <iostream>
#include <fstream>
#include <cstring>
#include <vector>
#include "GameEntity.h"

using namespace std;


class AssetLoader
{
public:
	AssetLoader();
	~AssetLoader();

	void testFunction();

	Mesh testFunction2();

	char cStringInput[512];

private:
	ifstream inputStream;
	ofstream outpitStream;
	vector<int> vertices;

};

