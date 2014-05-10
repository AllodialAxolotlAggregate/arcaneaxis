#include "Material.h"
#define delete_s(x) if(x != NULL && x != nullptr) delete(x)
#define ReleaseMacro(x) { if(x){ x->Release(); x = 0; } }

using namespace DirectX;

Material::Material() :
	m_ShaderResourceView(nullptr),
	m_SamplerState(nullptr),
	m_PixelShader(nullptr),
	m_VertexShader(nullptr),
	m_InputLayout(nullptr),
	m_ConstantBuffer(nullptr),
	m_ConstantBufferData(nullptr),
	m_Device(nullptr),
	m_DeviceContext(nullptr)
{
}

Material::Material(ID3D11Device* device, ID3D11DeviceContext* deviceContext) :
	m_ShaderResourceView(nullptr),
	m_SamplerState(nullptr),
	m_PixelShader(nullptr),
	m_VertexShader(nullptr),
	m_InputLayout(nullptr),
	m_ConstantBuffer(nullptr),
	m_ConstantBufferData(nullptr),
	m_Device(device),
	m_DeviceContext(deviceContext)
{
}

Material::~Material()
{
	Release();
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

void Material::SetUpDraw()
{
	// Set up the input assembler
	m_DeviceContext->IASetInputLayout(m_InputLayout);
	m_DeviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

	// Set up the Vertex and Pixel Shader
	m_DeviceContext->VSSetShader(m_VertexShader, NULL, 0);
	m_DeviceContext->PSSetShader(m_PixelShader, NULL, 0);
	m_DeviceContext->PSSetSamplers(0, 1, &m_SamplerState);
}

void Material::ActuallyDraw(XMFLOAT4X4 worldMatrix)
{
	//// Update the constant buffer
	//m_ConstantBufferData->world	= worldMatrix;

	//// Update the constant buffer
	//m_DeviceContext->UpdateSubresource(
	//	m_ConstantBuffer,
	//	0,			
	//	NULL,
	//	&(*m_ConstantBufferData),
	//	0,
	//	0);

	//// Set the current vertex and pixel shaders, as well the constant buffer for the vert shader
	//m_DeviceContext->VSSetConstantBuffers(
	//	0,	// Corresponds to the constant buffer's register in the vertex shader
	//	1, 
	//	&m_ConstantBuffer);

	// Set up the Shader Resource View and the Sampler State
	m_DeviceContext->PSSetShaderResources(0, 1, &m_ShaderResourceView);
}

void Material::Draw(XMFLOAT4X4 worldMatrix)
{
	SetUpDraw();
	ActuallyDraw(worldMatrix);
}

void Material::LoadSamplerStateAndShaderResourceView(const wchar_t* fileLocationName)
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

	// http://my.safaribooksonline.com/book/programming/game-programming/9781435458956/2d-rendering/ch03lev1sec4
	m_Device->CreateSamplerState(&samplerDesc, &m_SamplerState);
	CreateWICTextureFromFile(m_Device, m_DeviceContext, fileLocationName, 0, &m_ShaderResourceView);
}

void Material::LoadShadersAndInputLayout(const wchar_t* vsFile, const wchar_t* psFile, D3D11_INPUT_ELEMENT_DESC* vertexDesc, SIZE_T arraySize)
{
	ID3DBlob *vsBlob, *psBlob;
	D3DReadFileToBlob(vsFile, &vsBlob);
	D3DReadFileToBlob(psFile, &psBlob);

	// Create the shader on the device
	m_Device->CreateVertexShader(
		vsBlob->GetBufferPointer(),
		vsBlob->GetBufferSize(),
		NULL,
		&m_VertexShader);

	// Before cleaning up the data, create the input layout
	m_Device->CreateInputLayout(
		vertexDesc,
		arraySize,
		vsBlob->GetBufferPointer(),
		vsBlob->GetBufferSize(),
		&m_InputLayout);

	// Create the shader on the device
	m_Device->CreatePixelShader(
		psBlob->GetBufferPointer(),
		psBlob->GetBufferSize(),
		NULL,
		&m_PixelShader);

	ReleaseMacro(vsBlob);
	ReleaseMacro(psBlob);
}

void Material::Release()
{
	ReleaseMacro(m_SamplerState);
	ReleaseMacro(m_ShaderResourceView);
	ReleaseMacro(m_VertexShader);
	ReleaseMacro(m_PixelShader);
	ReleaseMacro(m_InputLayout);
	/*ReleaseMacro(m_Device);
	ReleaseMacro(m_DeviceContext);*/

	if(m_ConstantBuffer != nullptr)
		m_ConstantBuffer = nullptr;
	if(m_ConstantBufferData != nullptr)
		m_ConstantBufferData = nullptr;
}

void Material::LoadAConstantBuffer(ID3D11Buffer* aCSBuffer, VertexShaderConstantBuffer* aConstantBufferData)
{
	m_ConstantBuffer = aCSBuffer;
	m_ConstantBufferData = aConstantBufferData;
}