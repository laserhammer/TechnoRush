#include "AssetLoader.h"
#include <vector>
#include <array>
#include <iostream>
#include <fstream>
#include <cstring>


GameEntity* AssetLoader::LoadOBJ(ID3D11Device *device, VSConstantBufferLayout *constantBufferLayout, Material* material, ifstream& in_Stream)
{

	// I took the colors for the game entity creation
	XMFLOAT4 red = XMFLOAT4(1.0f, 0.0f, 0.0f, 1.0f);
	XMFLOAT4 green = XMFLOAT4(0.0f, 1.0f, 0.0f, 1.0f);
	XMFLOAT4 blue = XMFLOAT4(0.0f, 0.0f, 1.0f, 1.0f);
	XMFLOAT4 white = XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f);



	// This is a vector for all of the vertex structs
	std::vector<Vertex> objVertexVec;

	// This is vector that holds the specific vertex positions
	std::vector<XMFLOAT3> vertPos;

	// This is a vector that holds the indices
	std::vector<std::array<UINT, 3>> indices;

	// I was having an issue with faces. Faces that had more than 3 verts....might have been an OBJ export issue
	//int vertCount;

	// This is for texture coordiantes (Not yet implemented)
	std::vector<XMFLOAT2> vertTexCoord;
	
	// This was recommended from the online resource
	bool hasTexCoord = false;

	std::vector<XMFLOAT3> norms;

	// This is to check the next character from the file
	char checkChar;

	// As long as the file hasn't ended
	while (!in_Stream.eof())
	{
		// Get the next character
		checkChar = in_Stream.get();

		// Depending on the character, do something
		switch (checkChar)
		{
			// Ignore .OBJ comments
			case '#':
				checkChar = in_Stream.get();
				while (checkChar != '\n')
					checkChar = in_Stream.get();
				break;

			// This means that something has to do with a vertex
			case 'v':
				checkChar = in_Stream.get();

				// If it was just a 'v' then it's a vertex position
				if (checkChar == ' ')
				{
					// Read in the 3 positions per axis and put it in a XMFLOAT3
					float vz, vy, vx;
					in_Stream >> vx >> vy >> vz;

					vertPos.push_back(XMFLOAT3(vx, vy, vz * -1.0f));

				}

				// If the next character was 't', the is was actually "vt" which means that this a texture coordinate
				if (checkChar == 't')
				{
					// Read in the 2 coordinates and put it in a XMFLOAT2
					float vtcu, vtcv;
					in_Stream >> vtcu >> vtcv;

					vertTexCoord.push_back(XMFLOAT2(vtcu, vtcv));

					// BTW, this has OBJ officially has a texture
					hasTexCoord = true;
				}

				// if the next character is a 'n' then it's a normal
				if (checkChar == 'n')
				{
					float vnx, vny, vnz;
					in_Stream >> vnx >> vny >> vnz;

					norms.push_back(XMFLOAT3(vnx, vny, vnz));
				}
				break;
			case 'f':
			{
				// for this 'f'ace we athe start have 0 vertices
				//Parse by ' '
				while ((checkChar = in_Stream.get()) != ' ');
				// We know that there will be exactly 3 sets of indicies to make one tri
				char verts[3][32];
				int vertsLength = 0;
				for (int i = 0; i < 3; ++i)
				{
					UINT index[] = { 0, 0, 0 };
					while ((checkChar = in_Stream.get()) != ' ' && checkChar != '\n')
					{
						verts[i][vertsLength++] = checkChar;
					}
					// Now parse by '/' and store values into index array vector
					// Current index index(if that makes sense)
					int k = 2;
					// Current power of ten of the current index index(again, if that makes sense)
					int digit = 0;
					//for (int j = 0; j < vertsLengths[i]; ++j)
					do
					{
						// Get int value of the current char relative to '0'
						int temp = verts[i][vertsLength - 1] - 48;
						// If we have a slash go to the next index index and reset the current digit
						bool slash = temp < 0;
						k -= slash;
						// If we have a slash, reset the current digit
						digit *= !slash;
						// Multiply the current value to the power of 10 of the current digit
						for (int j = 0; j < digit; ++j)
						{
							temp *= 10;
						}
						// else increment the current digit
						digit += !slash;

						// Increment the current index index by the current ascii value depending on what digit we're on
						index[k] += temp * !slash;
					} while (--vertsLength);
					indices.push_back(std::array<UINT, 3>());
					indices[indices.size() - 1][0] = index[0];
					indices[indices.size() - 1][1] = index[1];
					indices[indices.size() - 1][2] = index[2];
				}

				
			}
				break;
			default:
				// If this line has nothing useful, just skip it. This default makes the '#' case irrelevant....but it's still there anyway
				while (checkChar != '\n' && !in_Stream.eof())
					checkChar = in_Stream.get();
				break;
		}



	}

	// The vector for the struct Vertex is now the right size
	objVertexVec.resize(vertPos.size());

	// Create a pointer to a new game entity
	GameEntity* newOBJEntity = new GameEntity(&vertPos, &indices, &vertTexCoord, &norms, device, constantBufferLayout, material);

	// Return the poiter to the new game entity
	return newOBJEntity;

}
