#pragma once

#include "DirectXGame.h"
#include <DirectXMath.h>

#if defined(DEBUG) || defined(_DEBUG)
#define _CRTDBG_MAP_ALLOC
#include <crtdbg.h>
#endif

struct PhongPixelShaderCbuffer
{
	DirectX::XMFLOAT4 lightColor;
	DirectX::XMFLOAT4 ka;
	DirectX::XMFLOAT4 kd;
	DirectX::XMFLOAT4 ks;
	DirectX::XMFLOAT4 shininess;
};

struct VSLightConstantBuffer;

class Material
{
public:
	// Special Memeber functions
	Material(ID3D11ShaderResourceView* textureView, ID3D11SamplerState* samplerState, 
		ID3D11VertexShader* vertexShader, ID3D11Buffer* vsConstantBuffer, ID3D11Buffer* lightBuffer, VSLightConstantBuffer* lightData,
		ID3D11PixelShader* pixelShader, ID3D11InputLayout* inputLayout, ID3D11Device* device);
	~Material();

	// Public Member fuctions
	ID3D11ShaderResourceView* textureView();
	ID3D11SamplerState* samplerState();
	ID3D11VertexShader* vertexShader();
	ID3D11PixelShader* pixelShader();
	ID3D11InputLayout* inputLayout();
	ID3D11Buffer* vsConstantBuffer();
	ID3D11Buffer* psConstantBuffer();
	PhongPixelShaderCbuffer psData();
	ID3D11Buffer* lightBuffer();
	VSLightConstantBuffer* lightData();
private:
	// Member functions
	//void loadTexture(const wchar_t* path, ID3D11Device* device, ID3D11DeviceContext* deviceContext);
private:

	// Member variables
	ID3D11ShaderResourceView* _textureView;
	ID3D11SamplerState* _samplerState;
	ID3D11VertexShader* _vertexShader;
	ID3D11PixelShader* _pixelShader;
	ID3D11InputLayout* _inputLayout;
	ID3D11Buffer* _vsConstantBuffer;
	ID3D11Buffer* _psConstantBuffer;
	PhongPixelShaderCbuffer _psData;
	ID3D11Buffer* _lightBuffer;
	VSLightConstantBuffer* _lightData;
};