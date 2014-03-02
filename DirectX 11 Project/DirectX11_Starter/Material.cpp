#include "Material.h"

using namespace DirectX;

Material::Material()
{
	r_ShaderResourceView = nullptr;
	r_SamplerState = nullptr;
}

Material::Material(ID3D11Device* device, ID3D11DeviceContext* deviceContext, ID3D11SamplerState* otherSampler, const wchar_t* fileLocationName)
{
	r_SamplerState = otherSampler;
	CreateWICTextureFromFile(device, deviceContext, fileLocationName, 0, &r_ShaderResourceView);
}

Material::~Material() {}

ID3D11ShaderResourceView* Material::GetShaderResourceView() { return r_ShaderResourceView; }
void Material::SetShaderResourceView(ID3D11ShaderResourceView* oSRV) { r_ShaderResourceView = oSRV; }

ID3D11SamplerState* Material::GetSamplerState() { return r_SamplerState; }
void Material::SetSamplerState(ID3D11SamplerState* oSampler) { r_SamplerState = oSampler; }