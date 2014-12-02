#pragma once
#include "Material.h"
#include "DirectXGame.h"
#include <DirectXMath.h>

struct Atlas
{
	DirectX::XMFLOAT2 displacement;
	DirectX::XMFLOAT2 scale;
};

class AtlasMaterial :
	public Material
{
public:
	AtlasMaterial(ID3D11ShaderResourceView* textureView, ID3D11SamplerState* samplerState,
		ID3D11VertexShader* vertexShader, ID3D11Buffer* vsConstantBuffer,
		ID3D11PixelShader* pixelShader, ID3D11InputLayout* inputLayout, ID3D11ShaderResourceView* normalMap, ID3D11Device* device, ID3D11DeviceContext* deviceContext);
	~AtlasMaterial();

	void SetDisplacement(DirectX::XMFLOAT2 displacement);
	void SetScale(DirectX::XMFLOAT2 scale);
	void UpdateBuffer();

private:
	ID3D11DeviceContext* deviceContext;
	ID3D11Buffer* _atlasBuffer;
	Atlas _atlasData;
};

