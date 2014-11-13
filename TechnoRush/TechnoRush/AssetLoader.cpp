#include "AssetLoader.h"
#include <vector>
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
	std::vector<UINT> indices;

	// I was having an issue with faces. Faces that had more than 3 verts....might have been an OBJ export issue
	int vertCount;

	// This is for texture coordiantes (Not yet implemented)
	std::vector<XMFLOAT2> vertTexCoord;
	
	// This was recommended from the online resource
	bool hasTexCoord = false;

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
				break;
			case 'f':

				// for this 'f'ace we athe start have 0 vertices
				vertCount = 0;

				// While the character is not the end of the line
				while (checkChar != '\n' && !in_Stream.eof())
				{
					// Get the next character
					checkChar = in_Stream.get();

					// If the character is a space and you have less than 3 vertices
					if (checkChar == ' ' && vertCount < 3)
					{
						// Get the next character (which should be a number
						checkChar = in_Stream.get();

						// If it is indeed a number
						if (isdigit(checkChar))
						{
							// Store the number
							char firstChar = checkChar;

							// Get the next character
							checkChar = in_Stream.get();

							// If the next character is a number
							if (isdigit(checkChar))
							{
								// Get both digits
								indices.push_back((((firstChar - '0') * 10) + (checkChar - '0')) - 1);
							}
							else
							{
								// If it wasn't a number, just do the on digit
								indices.push_back(firstChar - '0' - 1);
							}
							// We only want 3 verts
							vertCount++;
						}
						
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


	// Assemble each Vertex in the Vextex struct. Right now all the texture coordinates are the same
	for (int i = 0; i < vertPos.size(); i++)
	{
		objVertexVec[i] = { vertPos[i], white, XMFLOAT2(0.0f, 0.0f) };
	}

	// Create a pointer to a new game entity
	GameEntity* newOBJEntity = new GameEntity(objVertexVec, indices, device, constantBufferLayout, material);

	// Return the poiter to the new game entity
	return newOBJEntity;

}
