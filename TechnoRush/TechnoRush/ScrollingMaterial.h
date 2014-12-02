#pragma once
#include "Material.h"
#include "DirectXGame.h"
#include <DirectXMath.h>

struct Displacement
{
	DirectX::XMFLOAT2 scroll;
	DirectX::XMFLOAT2 filler;
};

class ScrollingMaterial :
	public Material
{
public:
	ScrollingMaterial(ID3D11ShaderResourceView* textureView, ID3D11SamplerState* samplerState,
		ID3D11VertexShader* vertexShader, ID3D11Buffer* vsConstantBuffer,
		ID3D11PixelShader* pixelShader, ID3D11InputLayout* inputLayout, ID3D11ShaderResourceView* normalMap,
		ID3D11Device* device, ID3D11DeviceContext* deviceContext);
	~ScrollingMaterial();

	void SetScroll(DirectX::XMFLOAT2 scroll);

private:
	ID3D11DeviceContext* deviceContext;
	Displacement _scrollData;
	ID3D11Buffer* _displacement;
};

