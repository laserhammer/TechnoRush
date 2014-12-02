
#include "Material.h"
#include "AssetLoader.h"
//#include "WICTextureLoader.h"



Material::Material(ID3D11ShaderResourceView* textureView, ID3D11SamplerState* samplerState,
	ID3D11VertexShader* vertexShader, ID3D11Buffer* vsConstantBuffer,
	ID3D11PixelShader* pixelShader, ID3D11InputLayout* inputLayout, ID3D11ShaderResourceView* normalMap)
{
	_textureView = textureView;
	_samplerState = samplerState;
	_vertexShader = vertexShader;
	_vsConstantBuffer = vsConstantBuffer;
	_pixelShader = pixelShader;
	_inputLayout = inputLayout;
	_normalMap = normalMap;
}

Material::~Material()
{
}
ID3D11ShaderResourceView* Material::textureView() { return _textureView; }
ID3D11ShaderResourceView* Material::normalMap() { return _normalMap; }
ID3D11SamplerState* Material::samplerState() { return _samplerState; }
ID3D11VertexShader* Material::vertexShader() { return _vertexShader; }
ID3D11PixelShader* Material::pixelShader() { return _pixelShader; }
ID3D11InputLayout* Material::inputLayout() { return _inputLayout; }
ID3D11Buffer* Material::vsConstantBuffer() { return _vsConstantBuffer; }