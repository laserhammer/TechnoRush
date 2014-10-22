#pragma once

#include "DirectXGame.h"

#if defined(DEBUG) || defined(_DEBUG)
#define _CRTDBG_MAP_ALLOC
#include <crtdbg.h>
#endif

class Material
{
public:
	// Special Memeber functions
	Material(ID3D11ShaderResourceView* textureView, ID3D11SamplerState* samplerState);
	~Material();

	// Public Member fuctions
	ID3D11ShaderResourceView* textureView();
	ID3D11SamplerState* samplerState();
private:
	// Member functions
	//void loadTexture(const wchar_t* path, ID3D11Device* device, ID3D11DeviceContext* deviceContext);
private:
	// Member variables
	ID3D11ShaderResourceView* _textureView;
	ID3D11SamplerState* _samplerState;
};