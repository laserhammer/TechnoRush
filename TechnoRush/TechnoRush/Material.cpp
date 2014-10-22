#include "Material.h"


Material::Material(ID3D11ShaderResourceView* textureView, ID3D11SamplerState* samplerState)
{
	_textureView = textureView;
	_samplerState = samplerState;
}

Material::~Material()
{
	ReleaseMacro(_textureView);
	ReleaseMacro(_samplerState);
}
ID3D11ShaderResourceView* Material::textureView() { return _textureView; }
ID3D11SamplerState* Material::samplerState() { return _samplerState; }