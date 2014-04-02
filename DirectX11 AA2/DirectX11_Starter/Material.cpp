#include "Material.h"
#define delete_s(x) if(x != NULL && x != nullptr) delete(x)
#define ReleaseMacro(x) { if(x){ x->Release(); x = 0; } }

using namespace DirectX;

Material::Material()
{
	m_ShaderResourceView = nullptr;
	m_SamplerState = nullptr;
	m_PixelShader = nullptr;
	m_VertexShader = nullptr;
	m_InputLayout = nullptr;
	m_ConstantBuffer = nullptr;
}

D3D11_SAMPLER_DESC Material::SamplerDescription()
{
	D3D11_SAMPLER_DESC samplerDesc;
	samplerDesc.Filter = D3D11_FILTER_MIN_MAG_MIP_LINEAR;
	samplerDesc.AddressU = D3D11_TEXTURE_ADDRESS_WRAP;
	samplerDesc.AddressV = D3D11_TEXTURE_ADDRESS_WRAP;
	samplerDesc.AddressW = D3D11_TEXTURE_ADDRESS_WRAP;
	samplerDesc.MipLODBias = 0;
	samplerDesc.MaxAnisotropy = 0;
	samplerDesc.MinLOD = 0;
	samplerDesc.MaxLOD = 0;

	return samplerDesc;
}

Material::Material(ID3D11Device* device, ID3D11DeviceContext* deviceContext, const wchar_t* fileLocationName)
{
	// http://my.safaribooksonline.com/book/programming/game-programming/9781435458956/2d-rendering/ch03lev1sec4
	D3D11_SAMPLER_DESC samplerDesc = SamplerDescription();
	device->CreateSamplerState(&samplerDesc, &m_SamplerState);
	CreateWICTextureFromFile(device, deviceContext, fileLocationName, 0, &m_ShaderResourceView);
}

Material::~Material()
{
	Release();
}

void Material::Draw(ID3D11DeviceContext* deviceContext)
{
	deviceContext->PSSetShaderResources(0, 1, &m_ShaderResourceView);
	deviceContext->PSSetSamplers(0, 1, &m_SamplerState);
}

void Material::LoadShadersAndInputLayout(ID3D11Device* device, ID3DBlob* vsBlob, ID3DBlob* psBlob, D3D11_INPUT_ELEMENT_DESC* vertexDesc, SIZE_T arraySize)
{
	// Create the shader on the device
	device->CreateVertexShader(
		vsBlob->GetBufferPointer(),
		vsBlob->GetBufferSize(),
		NULL,
		&m_VertexShader);

	// Before cleaning up the data, create the input layout
	device->CreateInputLayout(
		vertexDesc,
		arraySize,
		vsBlob->GetBufferPointer(),
		vsBlob->GetBufferSize(),
		&m_InputLayout);

	// Create the shader on the device
	device->CreatePixelShader(
		psBlob->GetBufferPointer(),
		psBlob->GetBufferSize(),
		NULL,
		&m_PixelShader);
}

void Material::Release()
{
	ReleaseMacro(m_SamplerState);
	ReleaseMacro(m_ShaderResourceView);
	ReleaseMacro(m_VertexShader);
	ReleaseMacro(m_PixelShader);
	ReleaseMacro(m_InputLayout);
}

void Material::LoadAConstantBuffer(ID3D11Buffer* aCSBuffer)
{
	m_ConstantBuffer = aCSBuffer;
}

ID3D11ShaderResourceView* Material::GetShaderResourceView() { return m_ShaderResourceView; }
void Material::SetShaderResourceView(ID3D11ShaderResourceView* oSRV) { m_ShaderResourceView = oSRV; }

ID3D11SamplerState* Material::GetSamplerState() { return m_SamplerState; }
void Material::SetSamplerState(ID3D11SamplerState* oSampler) { m_SamplerState = oSampler; }

ID3D11PixelShader* Material::GetPixelShader() { return m_PixelShader; }
void Material::SetPixelShader(ID3D11PixelShader* otherPixelShader) { m_PixelShader = otherPixelShader; }

ID3D11VertexShader* Material::GetVertexShader() { return m_VertexShader; }
void Material::SetVertexShader(ID3D11VertexShader* otherVertexShader) { m_VertexShader = otherVertexShader; }

ID3D11InputLayout* Material::GetInputLayout() { return m_InputLayout; }
void Material::SetInputLayer(ID3D11InputLayout* otherInputLayout) { m_InputLayout = otherInputLayout; }

ID3D11Buffer* Material::GetConstantBuffer() { return m_ConstantBuffer; }
void Material::SetConstantBuffer(ID3D11Buffer* otherConstantBuffer) { m_ConstantBuffer = otherConstantBuffer; }