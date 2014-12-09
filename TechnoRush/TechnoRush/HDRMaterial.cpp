#include "HDRMaterial.h"


HDRMaterial::HDRMaterial(ID3D11ShaderResourceView* textureView, ID3D11SamplerState* samplerState,
	ID3D11VertexShader* vertexShader, ID3D11Buffer* vsConstantBuffer,
	ID3D11PixelShader* pixelShader, ID3D11PixelShader* radialBlurShader, ID3D11InputLayout* inputLayout,
	ID3D11ShaderResourceView* normalMap, ID3D11Device* device, ID3D11DeviceContext* deviceContext)
	:Material(textureView, samplerState, vertexShader, vsConstantBuffer, pixelShader, inputLayout, normalMap)
{
	this->deviceContext = deviceContext;
	_stageData.stage = FILTER;

	D3D11_BUFFER_DESC cBufferDesc;
	cBufferDesc.ByteWidth = sizeof(StageDataBuffer);
	cBufferDesc.Usage = D3D11_USAGE_DEFAULT;
	cBufferDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	cBufferDesc.CPUAccessFlags = 0;
	cBufferDesc.MiscFlags = 0;
	cBufferDesc.StructureByteStride = 0;
	HR(device->CreateBuffer(&cBufferDesc, NULL, &_stageBuffer));

	deviceContext->PSSetConstantBuffers(3, 1, &(_stageBuffer));

	_radialBlur = false;
	_hDRShader = pixelShader;
	_radialBlurShader = radialBlurShader;
}

HDRMaterial::~HDRMaterial()
{
	this->Material::~Material();
	ReleaseMacro(_stageBuffer);
}

void HDRMaterial::SetStage(UINT stage)
{
	_stageData.stage = stage;
	_radialBlur = false;
}

void HDRMaterial::SetRadialBlur(float blur)
{
	_stageData.blur = blur;
	_radialBlur = true;
}

void HDRMaterial::UpdateBuffer()
{
	if (_radialBlur)
	{
		_pixelShader = _radialBlurShader;
	}
	else
	{
		_pixelShader = _hDRShader;
	}
	deviceContext->UpdateSubresource(_stageBuffer, 0, NULL, &_stageData, 0, 0);
}