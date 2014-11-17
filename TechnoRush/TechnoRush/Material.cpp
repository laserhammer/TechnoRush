
#include "Material.h"
#include "WICTextureLoader.h"
#include <DirectXMath.h>

struct VSLightConstantBuffer
{
	DirectX::XMFLOAT4 lightPos;
};

Material::Material(ID3D11ShaderResourceView* textureView, ID3D11SamplerState* samplerState,
	ID3D11VertexShader* vertexShader, ID3D11Buffer* vsConstantBuffer, ID3D11Buffer* lightBuffer, VSLightConstantBuffer* lightData,
	ID3D11PixelShader* pixelShader, ID3D11InputLayout* inputLayout, ID3D11Device* device)
{
	_textureView = textureView;
	_samplerState = samplerState;
	_vertexShader = vertexShader;
	_vsConstantBuffer = vsConstantBuffer;
	_pixelShader = pixelShader;
	_inputLayout = inputLayout;
	_lightBuffer = lightBuffer;
	_lightData = lightData;
	_psData = PhongPixelShaderCbuffer();
	_psData.lightColor = DirectX::XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f);
	_psData.ka = DirectX::XMFLOAT4(0.01f, 0.01f, 0.01f, 0.01f);
	_psData.kd = DirectX::XMFLOAT4(0.01f, 0.01f, 0.01f, 0.01f);
	_psData.ks = DirectX::XMFLOAT4(0.01f, 0.01f, 0.01f, 0.01f);
	_psData.shininess = DirectX::XMFLOAT4(100.0f, 1.0f, 1.0f, 1.0f);

	D3D11_BUFFER_DESC cBufferDesc;
	cBufferDesc.ByteWidth = sizeof(_psData);
	cBufferDesc.Usage = D3D11_USAGE_DEFAULT;
	cBufferDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	cBufferDesc.CPUAccessFlags = 0;
	cBufferDesc.MiscFlags = 0;
	cBufferDesc.StructureByteStride = 0;
	HR(device->CreateBuffer(&cBufferDesc, NULL, &_psConstantBuffer));
}

Material::~Material()
{
	ReleaseMacro(_psConstantBuffer);
}
ID3D11ShaderResourceView* Material::textureView() { return _textureView; }
ID3D11SamplerState* Material::samplerState() { return _samplerState; }
ID3D11VertexShader* Material::vertexShader() { return _vertexShader; }
ID3D11PixelShader* Material::pixelShader() { return _pixelShader; }
ID3D11InputLayout* Material::inputLayout() { return _inputLayout; }
ID3D11Buffer* Material::vsConstantBuffer() { return _vsConstantBuffer; }
ID3D11Buffer* Material::psConstantBuffer() { return _psConstantBuffer; }
PhongPixelShaderCbuffer Material::psData() { return _psData; }
ID3D11Buffer* Material::lightBuffer() { return _lightBuffer; }
VSLightConstantBuffer* Material::lightData() { return _lightData; }