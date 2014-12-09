#include "AssetLoader.h"
#include <vector>
#include <array>
#include <iostream>
#include <fstream>
#include <cstring>
#include <ctime>
#include "WICTextureLoader.h"
#include "DirectXGame.h"
#include <d3dcompiler.h>

Mesh* AssetLoader::sphere = NULL;
Mesh* AssetLoader::quad = NULL;
Mesh* AssetLoader::cube = NULL;
Mesh* AssetLoader::tower = NULL;
Mesh* AssetLoader::player = NULL;
Mesh* AssetLoader::floor = NULL;

Material* AssetLoader::playerMat = NULL;
ScrollingMaterial* AssetLoader::floorMat = NULL;
Material* AssetLoader::obstacleMat= NULL;
Material* AssetLoader::obstacleTowerMat = NULL;
Material* AssetLoader::scoreBackMat = NULL;
AtlasMaterial* AssetLoader::uiMat = NULL;
Material* AssetLoader::backgroundMat = NULL;
HDRMaterial* AssetLoader::hDRMat0 = NULL;
HDRMaterial* AssetLoader::hDRMat1 = NULL;
HDRMaterial* AssetLoader::hDRMat2 = NULL;

ifstream AssetLoader::in_Stream;

ID3D11SamplerState* AssetLoader::samplerState;
ID3D11ShaderResourceView* AssetLoader::playerTex;
ID3D11ShaderResourceView* AssetLoader::floorTex;
ID3D11ShaderResourceView* AssetLoader::floorNormMap;
ID3D11ShaderResourceView* AssetLoader::obstacleTex;
ID3D11ShaderResourceView* AssetLoader::obstacleTowerTex;
ID3D11ShaderResourceView* AssetLoader::black;
ID3D11ShaderResourceView* AssetLoader::uiTex;
ID3D11ShaderResourceView* AssetLoader::backgroundTex;
ID3D11ShaderResourceView* AssetLoader::pPTex0;
ID3D11ShaderResourceView* AssetLoader::pPTex1;
ID3D11ShaderResourceView* AssetLoader::pPTex2;

ID3D11Texture2D* AssetLoader::pPTex2D0;
ID3D11Texture2D* AssetLoader::pPTex2D1;
ID3D11Texture2D* AssetLoader::pPTex2D2;

ID3D11VertexShader* AssetLoader::playerVertexShader;
ID3D11PixelShader* AssetLoader::playerPixelShader;
ID3D11VertexShader* AssetLoader::obstacleVertexShader;
ID3D11PixelShader* AssetLoader::obstaclePixelShader;
ID3D11VertexShader* AssetLoader::floorVertexShader;
ID3D11PixelShader* AssetLoader::floorPixelShader;
ID3D11VertexShader* AssetLoader::uiVertexShader;
ID3D11PixelShader* AssetLoader::uiPixelShader;
ID3D11VertexShader* AssetLoader::backgroundVertexShader;
ID3D11PixelShader* AssetLoader::backgroundPixelShader;
ID3D11VertexShader* AssetLoader::hDRVertexShader;
ID3D11PixelShader* AssetLoader::hDRPixelShader;
ID3D11PixelShader* AssetLoader::radialBlurShader;

ID3D11InputLayout* AssetLoader::playerShaderInputLayout;
ID3D11InputLayout* AssetLoader::obstacleShaderInputLayout;
ID3D11InputLayout* AssetLoader::floorShaderInputLayout;
ID3D11InputLayout* AssetLoader::uiShaderInputLayout;
ID3D11InputLayout* AssetLoader::backgroundShaderInputLayout;

ID3D11Buffer* AssetLoader::vsConstantBuffer;
VSConstantBufferLayout AssetLoader::vsData;

void AssetLoader::LoadAssets(ID3D11Device* device, ID3D11DeviceContext* deviceContext)
{
	std::vector<XMFLOAT3> positions;
	std::vector<std::array<UINT, 3>> indices;
	std::vector<XMFLOAT2> vertTexCoord;
	std::vector<XMFLOAT3> norms;

	std::srand((unsigned int)time(0));

	in_Stream.open("../Resources/Meshes/Quad.obj");
	LoadMesh(positions, indices, vertTexCoord, norms);
	quad = new Mesh(&positions, &indices, &vertTexCoord, &norms, &XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f), device);
	in_Stream.close();

	positions.clear();
	indices.clear();
	vertTexCoord.clear();
	norms.clear();

	in_Stream.open("../Resources/Meshes/Cube.obj");
	LoadMesh(positions, indices, vertTexCoord, norms);
	cube = new Mesh(&positions, &indices, &vertTexCoord, &norms, &XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f), device);
	in_Stream.close();

	positions.clear();
	indices.clear();
	vertTexCoord.clear();
	norms.clear();

	in_Stream.open("../Resources/Meshes/Tower.obj");
	LoadMesh(positions, indices, vertTexCoord, norms);
	tower = new Mesh(&positions, &indices, &vertTexCoord, &norms, &XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f), device);
	in_Stream.close();

	positions.clear();
	indices.clear();
	vertTexCoord.clear();
	norms.clear();

	in_Stream.open("../Resources/Meshes/PlayerShip.obj");
	LoadMesh(positions, indices, vertTexCoord, norms);
	player = new Mesh(&positions, &indices, &vertTexCoord, &norms, &XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f), device);
	in_Stream.close();

	positions.clear();
	indices.clear();
	vertTexCoord.clear();
	norms.clear();

	in_Stream.open("../Resources/Meshes/Floor.obj");
	LoadMesh(positions, indices, vertTexCoord, norms);
	floor = new Mesh(&positions, &indices, &vertTexCoord, &norms, &XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f), device);
	in_Stream.close();

	D3D11_SAMPLER_DESC desc;
	//desc.Filter = D3D11_FILTER_MIN_MAG_MIP_POINT;
	desc.Filter = D3D11_FILTER_ANISOTROPIC;
	desc.AddressU = D3D11_TEXTURE_ADDRESS_WRAP;
	desc.AddressV = D3D11_TEXTURE_ADDRESS_WRAP;
	desc.AddressW = D3D11_TEXTURE_ADDRESS_WRAP;
	desc.BorderColor[0] = 0;
	desc.BorderColor[1] = 0;
	desc.BorderColor[2] = 0;
	desc.BorderColor[3] = 0;
	desc.ComparisonFunc = D3D11_COMPARISON_ALWAYS;
	desc.MaxAnisotropy = 8;
	desc.MaxLOD = 0;
	desc.MinLOD = 0;
	desc.MipLODBias = 0;

	device->CreateSamplerState(&desc, &samplerState);

	CreateWICTextureFromFile(device, deviceContext, L"../Resources/Textures/playerTexture.png", 0, &playerTex);
	CreateWICTextureFromFile(device, deviceContext, L"../Resources/Textures/FloorTex.png", 0, &floorTex);
	CreateWICTextureFromFile(device, deviceContext, L"../Resources/Textures/TileNormal.png", 0, &floorNormMap);
	CreateWICTextureFromFile(device, deviceContext, L"../Resources/Textures/CubeTexture.png", 0, &obstacleTex);
	CreateWICTextureFromFile(device, deviceContext, L"../Resources/Textures/TowerTexture.png", 0, &obstacleTowerTex);
	CreateWICTextureFromFile(device, deviceContext, L"../Resources/Textures/Black.png", 0, &black);
	CreateWICTextureFromFile(device, deviceContext, L"../Resources/Textures/UIText.png", 0, &uiTex);
	CreateWICTextureFromFile(device, deviceContext, L"../Resources/Textures/Background.png", 0, &backgroundTex);

	LoadShaderPair(playerVertexShader, L"VertexPhong.cso", playerPixelShader, L"IlluminatiPixelPhong.cso", playerShaderInputLayout, device);
	LoadShaderPair(obstacleVertexShader, L"VertexPhong.cso", obstaclePixelShader, L"IlluminatiPixelPhong.cso", obstacleShaderInputLayout, device);
	LoadShaderPair(floorVertexShader, L"ScrollingVertexPhong.cso", floorPixelShader, L"FloorPixelShader.cso", floorShaderInputLayout, device);
	LoadShaderPair(uiVertexShader, L"AtlasVertexShader.cso", uiPixelShader, L"AlphaCutoffPixelShader.cso", uiShaderInputLayout, device);
	LoadShaderPair(backgroundVertexShader, L"VertexShader.cso", backgroundPixelShader, L"PixelShader.cso", backgroundShaderInputLayout, device);
	LoadShaderPair(hDRVertexShader, L"HDRVertexShader.cso", hDRPixelShader, L"HDRPixelshader.cso", playerShaderInputLayout, device);
	LoadShaderPair(hDRVertexShader, L"HDRVertexShader.cso", radialBlurShader, L"RadialBlurShader.cso", playerShaderInputLayout, device);

	D3D11_BUFFER_DESC cBufferDesc;
	cBufferDesc.ByteWidth = sizeof(vsData);
	cBufferDesc.Usage = D3D11_USAGE_DEFAULT;
	cBufferDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	cBufferDesc.CPUAccessFlags = 0;
	cBufferDesc.MiscFlags = 0;
	cBufferDesc.StructureByteStride = 0;
	HR(device->CreateBuffer(&cBufferDesc, NULL, &vsConstantBuffer));

	playerMat = new Material(playerTex, samplerState, playerVertexShader, vsConstantBuffer, playerPixelShader, playerShaderInputLayout, NULL);
	obstacleMat = new Material(obstacleTex, samplerState, obstacleVertexShader, vsConstantBuffer, obstaclePixelShader, obstacleShaderInputLayout, NULL);
	obstacleTowerMat = new Material(obstacleTowerTex, samplerState, obstacleVertexShader, vsConstantBuffer, obstaclePixelShader, obstacleShaderInputLayout, NULL);
	floorMat = new ScrollingMaterial(floorTex, samplerState, floorVertexShader, vsConstantBuffer, floorPixelShader, floorShaderInputLayout, floorNormMap, device, deviceContext);
	scoreBackMat = new Material(black, samplerState, backgroundVertexShader, vsConstantBuffer, backgroundPixelShader, backgroundShaderInputLayout, NULL);
	uiMat = new AtlasMaterial(uiTex, samplerState, uiVertexShader, vsConstantBuffer, uiPixelShader, uiShaderInputLayout, NULL, device, deviceContext);
	backgroundMat = new Material(backgroundTex, samplerState, backgroundVertexShader, vsConstantBuffer, backgroundPixelShader, backgroundShaderInputLayout, NULL);

	// Render to texture resources
	// Based on code from http://www.braynzarsoft.net/index.php?p=D3D11RTT
	D3D11_TEXTURE2D_DESC texDesc;
	D3D11_SHADER_RESOURCE_VIEW_DESC resourceViewDesc;
	ZeroMemory(&texDesc, sizeof(texDesc));
	texDesc.Width = 800;
	texDesc.Height = 600;
	texDesc.MipLevels = 1;
	texDesc.ArraySize = 1;
	texDesc.Format = DXGI_FORMAT_R32G32B32A32_FLOAT;
	texDesc.SampleDesc.Count = 1;
	texDesc.Usage = D3D11_USAGE_DEFAULT;
	texDesc.BindFlags = D3D11_BIND_RENDER_TARGET | D3D11_BIND_SHADER_RESOURCE;
	texDesc.CPUAccessFlags = 0;
	texDesc.MiscFlags = 0;

	HR(device->CreateTexture2D(&texDesc, NULL, &pPTex2D0));
	HR(device->CreateTexture2D(&texDesc, NULL, &pPTex2D1));
	HR(device->CreateTexture2D(&texDesc, NULL, &pPTex2D2));

	resourceViewDesc.Format = texDesc.Format;
	resourceViewDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
	resourceViewDesc.Texture2D.MostDetailedMip = 0;
	resourceViewDesc.Texture2D.MipLevels = 1;

	HR(device->CreateShaderResourceView(pPTex2D0, &resourceViewDesc, &pPTex0));
	HR(device->CreateShaderResourceView(pPTex2D1, &resourceViewDesc, &pPTex1));
	HR(device->CreateShaderResourceView(pPTex2D2, &resourceViewDesc, &pPTex2));

	hDRMat0 = new HDRMaterial(pPTex0, samplerState, hDRVertexShader, vsConstantBuffer, hDRPixelShader, radialBlurShader, playerShaderInputLayout, NULL, device, deviceContext);
	hDRMat1 = new HDRMaterial(pPTex1, samplerState, hDRVertexShader, vsConstantBuffer, hDRPixelShader, radialBlurShader, playerShaderInputLayout, NULL, device, deviceContext);
	hDRMat2 = new HDRMaterial(pPTex2, samplerState, hDRVertexShader, vsConstantBuffer, hDRPixelShader, radialBlurShader, playerShaderInputLayout, NULL, device, deviceContext);
}

void AssetLoader::ReleaseAssets()
{
	ReleaseMacro(playerVertexShader);
	ReleaseMacro(playerPixelShader);
	ReleaseMacro(obstacleVertexShader);
	ReleaseMacro(obstaclePixelShader);
	ReleaseMacro(floorVertexShader);
	ReleaseMacro(floorPixelShader);
	ReleaseMacro(uiVertexShader);
	ReleaseMacro(uiPixelShader);
	ReleaseMacro(backgroundVertexShader);
	ReleaseMacro(backgroundPixelShader);
	ReleaseMacro(hDRVertexShader);
	ReleaseMacro(hDRPixelShader);

	ReleaseMacro(vsConstantBuffer);

	ReleaseMacro(playerShaderInputLayout);
	ReleaseMacro(obstacleShaderInputLayout);
	ReleaseMacro(floorShaderInputLayout);
	ReleaseMacro(uiShaderInputLayout);
	ReleaseMacro(backgroundShaderInputLayout);

	ReleaseMacro(playerTex);
	ReleaseMacro(obstacleTex);
	ReleaseMacro(obstacleTowerTex);
	ReleaseMacro(floorTex);
	ReleaseMacro(black);
	ReleaseMacro(uiTex);
	ReleaseMacro(backgroundTex);
	ReleaseMacro(pPTex0);
	ReleaseMacro(pPTex1);
	ReleaseMacro(pPTex2);

	ReleaseMacro(pPTex2D0);
	ReleaseMacro(pPTex2D1);
	ReleaseMacro(pPTex2D2);

	ReleaseMacro(samplerState);

	delete sphere;
	delete quad;
	delete cube;
	delete tower;
	delete player;
	delete floor;

	delete playerMat;
	delete floorMat;
	delete obstacleMat;
	delete obstacleTowerMat;
	delete scoreBackMat;
	delete uiMat;
	delete backgroundMat;
	delete hDRMat0;
	delete hDRMat1;
	delete hDRMat2;
}

void AssetLoader::LoadMesh(std::vector<XMFLOAT3> &positions, std::vector<std::array<UINT, 3>> &indices, std::vector<XMFLOAT2> &vertTexCoord, std::vector<XMFLOAT3> &norms)
{

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

				positions.push_back(XMFLOAT3(vx, vy, vz * -1.0f));

			}

			// If the next character was 't', the is was actually "vt" which means that this a texture coordinate
			if (checkChar == 't')
			{
				// Read in the 2 coordinates and put it in a XMFLOAT2
				float vtcu, vtcv;
				in_Stream >> vtcu >> vtcv;

				vertTexCoord.push_back(XMFLOAT2(vtcu, vtcv));

				// BTW, this has OBJ officially has a texture
				//hasTexCoord = true;
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
			int startIndex = indices.size();
			int indexModifier = 0;
			indices.resize(startIndex + 3);
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
				indexModifier = 0;
				indexModifier += (i > 0) * 2;
				indexModifier -= (i == 2);
				indices[startIndex + indexModifier][0] = index[0];
				indices[startIndex + indexModifier][1] = index[1];
				indices[startIndex + indexModifier][2] = index[2];
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
}

void AssetLoader::LoadShaderPair(ID3D11VertexShader* &vertexShader, LPCWSTR vertexShaderPath, ID3D11PixelShader* &pixelShader, LPCWSTR pixelShaderPath, ID3D11InputLayout* &inputLayout, ID3D11Device* device)
{
	ID3DBlob* vsBlob;
	D3DReadFileToBlob(vertexShaderPath, &vsBlob);
	HR(device->CreateVertexShader(vsBlob->GetBufferPointer(), vsBlob->GetBufferSize(), NULL, &vertexShader));
	CreateInputLayoutDescFromVertexShaderSignature(vsBlob, &inputLayout, device);

	ReleaseMacro(vsBlob);

	ID3DBlob* psBlob;
	D3DReadFileToBlob(pixelShaderPath, &psBlob);
	HR(device->CreatePixelShader(psBlob->GetBufferPointer(), psBlob->GetBufferSize(), NULL, &pixelShader));
	ReleaseMacro(psBlob);
}

void AssetLoader::CreateInputLayoutDescFromVertexShaderSignature(ID3DBlob* shaderBlob, ID3D11InputLayout** inputLayout, ID3D11Device* device)
{
	// Reflect shader info
	ID3D11ShaderReflection* vertexShaderReflection = NULL;
	HR(D3DReflect(shaderBlob->GetBufferPointer(), shaderBlob->GetBufferSize(), IID_ID3D11ShaderReflection, (void**)&vertexShaderReflection));

	// Get shader info
	D3D11_SHADER_DESC shaderDesc;
	vertexShaderReflection->GetDesc(&shaderDesc);

	// Read input layout description from shader info
	std::vector<D3D11_INPUT_ELEMENT_DESC> inputLayoutDesc;
	for (UINT32 i = 0; i < shaderDesc.InputParameters; ++i)
	{
		D3D11_SIGNATURE_PARAMETER_DESC paramDesc;
		vertexShaderReflection->GetInputParameterDesc(i, &paramDesc);

		// fill out input element desc
		D3D11_INPUT_ELEMENT_DESC elementDesc;
		elementDesc.SemanticName = paramDesc.SemanticName;
		elementDesc.SemanticIndex = paramDesc.SemanticIndex;
		elementDesc.InputSlot = 0;
		elementDesc.AlignedByteOffset = D3D11_APPEND_ALIGNED_ELEMENT;
		elementDesc.InputSlotClass = D3D11_INPUT_PER_VERTEX_DATA;
		elementDesc.InstanceDataStepRate = 0;

		// determine DXGI format
		if (paramDesc.Mask == 1)
		{
			if (paramDesc.ComponentType == D3D_REGISTER_COMPONENT_UINT32) elementDesc.Format = DXGI_FORMAT_R32_UINT;
			else if (paramDesc.ComponentType == D3D_REGISTER_COMPONENT_SINT32) elementDesc.Format = DXGI_FORMAT_R32_SINT;
			else if (paramDesc.ComponentType == D3D_REGISTER_COMPONENT_FLOAT32) elementDesc.Format = DXGI_FORMAT_R32_FLOAT;
		}
		else if (paramDesc.Mask <= 3)
		{
			if (paramDesc.ComponentType == D3D_REGISTER_COMPONENT_UINT32) elementDesc.Format = DXGI_FORMAT_R32G32_UINT;
			else if (paramDesc.ComponentType == D3D_REGISTER_COMPONENT_SINT32) elementDesc.Format = DXGI_FORMAT_R32G32_SINT;
			else if (paramDesc.ComponentType == D3D_REGISTER_COMPONENT_FLOAT32) elementDesc.Format = DXGI_FORMAT_R32G32_FLOAT;
		}
		else if (paramDesc.Mask <= 7)
		{
			if (paramDesc.ComponentType == D3D_REGISTER_COMPONENT_UINT32) elementDesc.Format = DXGI_FORMAT_R32G32B32_UINT;
			else if (paramDesc.ComponentType == D3D_REGISTER_COMPONENT_SINT32) elementDesc.Format = DXGI_FORMAT_R32G32B32_SINT;
			else if (paramDesc.ComponentType == D3D_REGISTER_COMPONENT_FLOAT32) elementDesc.Format = DXGI_FORMAT_R32G32B32_FLOAT;
		}
		else if (paramDesc.Mask <= 15)
		{
			if (paramDesc.ComponentType == D3D_REGISTER_COMPONENT_UINT32) elementDesc.Format = DXGI_FORMAT_R32G32B32A32_UINT;
			else if (paramDesc.ComponentType == D3D_REGISTER_COMPONENT_SINT32) elementDesc.Format = DXGI_FORMAT_R32G32B32A32_SINT;
			else if (paramDesc.ComponentType == D3D_REGISTER_COMPONENT_FLOAT32) elementDesc.Format = DXGI_FORMAT_R32G32B32A32_FLOAT;
		}

		// save element desc
		inputLayoutDesc.push_back(elementDesc);
	}

	// Try to create Input Layout
	HR(device->CreateInputLayout(&inputLayoutDesc[0], inputLayoutDesc.size(), shaderBlob->GetBufferPointer(), shaderBlob->GetBufferSize(), inputLayout));

	// Free allocation shader reflection memory
	ReleaseMacro(vertexShaderReflection);
}

