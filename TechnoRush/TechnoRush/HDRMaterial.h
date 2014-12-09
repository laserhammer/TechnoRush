#pragma once
#include "Material.h"
#include <DirectXMath.h>

struct StageDataBuffer
{
	UINT stage;
	float blur;
	DirectX::XMFLOAT2 filler;
};

enum Stage : UINT
{
	FILTER = 0,
	HORIZONTAL_BLUR = 1,
	VERTICAL_BLUR = 2,
	MERGE = 3
};

class HDRMaterial :
	public Material
{
public:
	HDRMaterial(ID3D11ShaderResourceView* textureView, ID3D11SamplerState* samplerState,
		ID3D11VertexShader* vertexShader, ID3D11Buffer* vsConstantBuffer,
		ID3D11PixelShader* pixelShader, ID3D11PixelShader* radialBlurShader, ID3D11InputLayout* inputLayout, 
		ID3D11ShaderResourceView* normalMap, ID3D11Device* device, ID3D11DeviceContext* deviceContext);
	~HDRMaterial();

	void SetStage(UINT dir);
	void SetRadialBlur(float blur);
	void UpdateBuffer();

private:
	ID3D11DeviceContext* deviceContext;
	ID3D11Buffer* _stageBuffer;
	StageDataBuffer _stageData;

	ID3D11PixelShader* _radialBlurShader;
	ID3D11PixelShader* _hDRShader;
	bool _radialBlur;
};

