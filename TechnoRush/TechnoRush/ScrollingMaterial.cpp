#include "ScrollingMaterial.h"


ScrollingMaterial::ScrollingMaterial(ID3D11ShaderResourceView* textureView, ID3D11SamplerState* samplerState,
	ID3D11VertexShader* vertexShader, ID3D11Buffer* vsConstantBuffer,
	ID3D11PixelShader* pixelShader, ID3D11InputLayout* inputLayout, ID3D11Device* device, ID3D11DeviceContext* deviceContext) 
	:Material(textureView, samplerState, vertexShader, vsConstantBuffer, pixelShader, inputLayout)
{
	this->deviceContext = deviceContext;
	_scrollData.scroll = DirectX::XMFLOAT2(0.0f, 0.0f);

	D3D11_BUFFER_DESC cBufferDesc;
	cBufferDesc.ByteWidth = sizeof(Displacement);
	cBufferDesc.Usage = D3D11_USAGE_DEFAULT;
	cBufferDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	cBufferDesc.CPUAccessFlags = 0;
	cBufferDesc.MiscFlags = 0;
	cBufferDesc.StructureByteStride = 0;
	HR(device->CreateBuffer(&cBufferDesc, NULL, &_displacement));

	deviceContext->VSSetConstantBuffers(1, 1, &(_displacement));
}


ScrollingMaterial::~ScrollingMaterial()
{
	this->Material::~Material();
	ReleaseMacro(_displacement);
}

void ScrollingMaterial::SetScroll(DirectX::XMFLOAT2 scroll)
{
	_scrollData.scroll = scroll; 
	deviceContext->UpdateSubresource(_displacement, 0, NULL, &_scrollData, 0, 0);
}