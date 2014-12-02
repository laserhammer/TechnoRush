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
	Material(ID3D11ShaderResourceView* textureView, ID3D11SamplerState* samplerState,
		ID3D11VertexShader* vertexShader, ID3D11Buffer* vsConstantBuffer,
		ID3D11PixelShader* pixelShader, ID3D11InputLayout* inputLayout, ID3D11ShaderResourceView* normalMap);
	~Material();

	// Public Member fuctions
	ID3D11ShaderResourceView* textureView();
	ID3D11ShaderResourceView* normalMap();
	ID3D11SamplerState* samplerState();
	ID3D11VertexShader* vertexShader();
	ID3D11PixelShader* pixelShader();
	ID3D11InputLayout* inputLayout();
	ID3D11Buffer* vsConstantBuffer();
private:
	// Member functions
	//void loadTexture(const wchar_t* path, ID3D11Device* device, ID3D11DeviceContext* deviceContext);
private:

	// Member variables
	ID3D11ShaderResourceView* _textureView;
	ID3D11ShaderResourceView* _normalMap;
	ID3D11SamplerState* _samplerState;
	ID3D11VertexShader* _vertexShader;
	ID3D11PixelShader* _pixelShader;
	ID3D11InputLayout* _inputLayout;
	ID3D11Buffer* _vsConstantBuffer;
};