#include "AtlasMaterial.h"


AtlasMaterial::AtlasMaterial(ID3D11ShaderResourceView* textureView, ID3D11SamplerState* samplerState,
	ID3D11VertexShader* vertexShader, ID3D11Buffer* vsConstantBuffer,
	ID3D11PixelShader* pixelShader, ID3D11InputLayout* inputLayout, ID3D11ShaderResourceView* normalMap, ID3D11Device* device, ID3D11DeviceContext* deviceContext)
	:Material(textureView, samplerState, vertexShader, vsConstantBuffer, pixelShader, inputLayout, normalMap)
{
	this->deviceContext = deviceContext;
	_atlasData.displacement = DirectX::XMFLOAT2(0.0f, 0.0f);
	_atlasData.scale = DirectX::XMFLOAT2(0.0f, 0.0f);

	D3D11_BUFFER_DESC cBufferDesc;
	cBufferDesc.ByteWidth = sizeof(Atlas);
	cBufferDesc.Usage = D3D11_USAGE_DEFAULT;
	cBufferDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	cBufferDesc.CPUAccessFlags = 0;
	cBufferDesc.MiscFlags = 0;
	cBufferDesc.StructureByteStride = 0;
	HR(device->CreateBuffer(&cBufferDesc, NULL, &_atlasBuffer));

	deviceContext->VSSetConstantBuffers(2, 1, &(_atlasBuffer));
}


AtlasMaterial::~AtlasMaterial()
{
	this->Material::~Material();
	ReleaseMacro(_atlasBuffer);
}

void AtlasMaterial::SetDisplacement(DirectX::XMFLOAT2 displacement) { _atlasData.displacement = displacement; }
void AtlasMaterial::SetScale(DirectX::XMFLOAT2 scale) { _atlasData.scale = scale; }
void AtlasMaterial::UpdateBuffer()
{
	deviceContext->UpdateSubresource(_atlasBuffer, 0, NULL, &_atlasData, 0, 0);
}
