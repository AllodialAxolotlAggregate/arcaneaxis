#include "FontShader.h"
using namespace DirectX;

FontShader::FontShader()
{
	m_VertexShader = nullptr;
	m_PixelShader = nullptr;
	m_Layout = nullptr;
	m_ConstantBuffer = nullptr;
	m_SamplerState = nullptr;
	m_PixelBuffer = nullptr;
}

void FontShader::Initialize(ID3D11Device* device, WCHAR* vsFilename, WCHAR* psFilename)
{
	ID3DBlob* vsBlob;
	ID3DBlob* psBlob;
	D3D11_INPUT_ELEMENT_DESC polygonLayout[2];
	D3D11_BUFFER_DESC constantBufferDesc;
    D3D11_SAMPLER_DESC samplerDesc;
	D3D11_BUFFER_DESC pixelBufferDesc;

	// Initialize the pointers this function will use to null.
	// errorMessage = 0;
	vsBlob = 0;
	psBlob = 0;

#pragma region Descriptions

	// Create the vertex input layout description.
	// This setup needs to match the VertexType stucture in the ModelClass and in the shader.
	polygonLayout[0].SemanticName = "POSITION";
	polygonLayout[0].SemanticIndex = 0;
	polygonLayout[0].Format = DXGI_FORMAT_R32G32B32_FLOAT;
	polygonLayout[0].InputSlot = 0;
	polygonLayout[0].AlignedByteOffset = 0;
	polygonLayout[0].InputSlotClass = D3D11_INPUT_PER_VERTEX_DATA;
	polygonLayout[0].InstanceDataStepRate = 0;

	polygonLayout[1].SemanticName = "TEXCOORD";
	polygonLayout[1].SemanticIndex = 0;
	polygonLayout[1].Format = DXGI_FORMAT_R32G32_FLOAT;
	polygonLayout[1].InputSlot = 0;
	polygonLayout[1].AlignedByteOffset = D3D11_APPEND_ALIGNED_ELEMENT;
	polygonLayout[1].InputSlotClass = D3D11_INPUT_PER_VERTEX_DATA;
	polygonLayout[1].InstanceDataStepRate = 0;

	// Setup the description of the dynamic constant buffer that is in the vertex shader.
    constantBufferDesc.Usage = D3D11_USAGE_DYNAMIC;
	constantBufferDesc.ByteWidth = sizeof(VertexShaderConstantBuffer);
    constantBufferDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
    constantBufferDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
    constantBufferDesc.MiscFlags = 0;
	constantBufferDesc.StructureByteStride = 0;

	// Create a texture sampler state description.
    samplerDesc.Filter = D3D11_FILTER_MIN_MAG_MIP_LINEAR;
    samplerDesc.AddressU = D3D11_TEXTURE_ADDRESS_WRAP;
    samplerDesc.AddressV = D3D11_TEXTURE_ADDRESS_WRAP;
    samplerDesc.AddressW = D3D11_TEXTURE_ADDRESS_WRAP;
    samplerDesc.MipLODBias = 0.0f;
    samplerDesc.MaxAnisotropy = 1;
    samplerDesc.ComparisonFunc = D3D11_COMPARISON_ALWAYS;
    samplerDesc.BorderColor[0] = 0;
	samplerDesc.BorderColor[1] = 0;
	samplerDesc.BorderColor[2] = 0;
	samplerDesc.BorderColor[3] = 0;
    samplerDesc.MinLOD = 0;
    samplerDesc.MaxLOD = D3D11_FLOAT32_MAX;

	// Setup the description of the dynamic pixel constant buffer that is in the pixel shader.
    pixelBufferDesc.Usage = D3D11_USAGE_DYNAMIC;
	pixelBufferDesc.ByteWidth = sizeof(PixelBufferType);
    pixelBufferDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
    pixelBufferDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
    pixelBufferDesc.MiscFlags = 0;
	pixelBufferDesc.StructureByteStride = 0;

#pragma endregion

	D3DReadFileToBlob(vsFilename, &vsBlob);
	device->CreateVertexShader(
		vsBlob->GetBufferPointer(),
		vsBlob->GetBufferSize(),
		NULL,
		&m_VertexShader);

	D3DReadFileToBlob(psFilename, &psBlob);
	device->CreatePixelShader(
		psBlob->GetBufferPointer(),
		psBlob->GetBufferSize(),
		NULL,
		&m_PixelShader);

	device->CreateInputLayout(
		polygonLayout,
		ARRAYSIZE(polygonLayout),
		vsBlob->GetBufferPointer(),
		vsBlob->GetBufferSize(),
		&m_Layout);

	device->CreateBuffer(
		&constantBufferDesc,
		NULL,
		&m_ConstantBuffer);

	// Create the texture sampler state.
	device->CreateSamplerState(&samplerDesc, &m_SamplerState);

	// Create the pixel constant buffer pointer so we can access the pixel shader constant buffer from within this class.
	device->CreateBuffer(&pixelBufferDesc, NULL, &m_PixelBuffer);

	// Release the vertex shader buffer and pixel shader buffer since they are no longer needed.
	vsBlob->Release();
	vsBlob = 0;

	psBlob->Release();
	psBlob = 0;
}

FontShader::~FontShader() {}

void FontShader::Shutdown() { ShutdownShader(); }

void FontShader::Render(ID3D11DeviceContext* deviceContext, int indexCount, XMFLOAT4X4 worldMatrix, XMFLOAT4X4 viewMatrix, 
							 XMFLOAT4X4 projectionMatrix, ID3D11ShaderResourceView* texture, XMFLOAT4 pixelColor)
{
	// Set the shader parameters that it will use for rendering
	SetShaderParameters(deviceContext, worldMatrix, viewMatrix, projectionMatrix, texture, pixelColor);

	// Now render the prepared buffers with the shader
	RenderShader(deviceContext, indexCount);
}

void FontShader::ShutdownShader()
{
	// Release the pixel constant buffer.
	if(m_PixelBuffer)
	{
		m_PixelBuffer->Release();
		m_PixelBuffer = 0;
	}

	// Release the sampler state.
	if(m_SamplerState)
	{
		m_SamplerState->Release();
		m_SamplerState = 0;
	}

	// Release the constant buffer.
	if(m_ConstantBuffer)
	{
		m_ConstantBuffer->Release();
		m_ConstantBuffer = 0;
	}

	// Release the layout.
	if(m_Layout)
	{
		m_Layout->Release();
		m_Layout = 0;
	}

	// Release the pixel shader.
	if(m_PixelShader)
	{
		m_PixelShader->Release();
		m_PixelShader = 0;
	}

	// Release the vertex shader.
	if(m_VertexShader)
	{
		m_VertexShader->Release();
		m_VertexShader = 0;
	}
}

void FontShader::SetShaderParameters(ID3D11DeviceContext* deviceContext, XMFLOAT4X4 worldMatrix, XMFLOAT4X4 viewMatrix, 
										  XMFLOAT4X4 projectionMatrix, ID3D11ShaderResourceView* texture, XMFLOAT4 pixelColor)
{
    D3D11_MAPPED_SUBRESOURCE mappedResource;
	VertexShaderConstantBuffer* dataPtr;
	unsigned int bufferNumber;
	PixelBufferType* dataPtr2;

	// Lock the constant buffer so it can be written to.
	deviceContext->Map(m_ConstantBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedResource);

	// Get a pointer to the data in the constant buffer.
	dataPtr = (VertexShaderConstantBuffer*)mappedResource.pData;

	// Transpose the matrices to prepare them for the shader.
	/*XMMATRIX w, v, p;
	w = XMLoadFloat4x4(&worldMatrix);
	XMStoreFloat4x4(&worldMatrix, XMMatrixTranspose(w));

	v = XMLoadFloat4x4(&viewMatrix);
	XMStoreFloat4x4(&viewMatrix, XMMatrixTranspose(v));

	p = XMLoadFloat4x4(&projectionMatrix);
	XMStoreFloat4x4(&projectionMatrix, XMMatrixTranspose(p));*/

	// Copy the matrices into the constant buffer.
	dataPtr->world = worldMatrix;
	dataPtr->view = viewMatrix;
	dataPtr->projection = projectionMatrix;

	// Unlock the constant buffer.
    deviceContext->Unmap(m_ConstantBuffer, 0);

	// Set the position of the constant buffer in the vertex shader.
	bufferNumber = 0;

	// Now set the constant buffer in the vertex shader with the updated values.
    deviceContext->VSSetConstantBuffers(bufferNumber, 1, &m_ConstantBuffer);

	// Set shader texture resource in the pixel shader.
	deviceContext->PSSetShaderResources(0, 1, &texture);

	// Lock the pixel constant buffer so it can be written to.
	deviceContext->Map(m_PixelBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedResource);

	// Get a pointer to the data in the pixel constant buffer.
	dataPtr2 = (PixelBufferType*)mappedResource.pData;

	// Copy the pixel color into the pixel constant buffer.
	dataPtr2->pixelColor = pixelColor;

	// Unlock the pixel constant buffer.
    deviceContext->Unmap(m_PixelBuffer, 0);

	// Set the position of the pixel constant buffer in the pixel shader.
	bufferNumber = 0;

	// Now set the pixel constant buffer in the pixel shader with the updated value.
    deviceContext->PSSetConstantBuffers(bufferNumber, 1, &m_PixelBuffer);
}


void FontShader::RenderShader(ID3D11DeviceContext* deviceContext, int indexCount)
{
	// Set the vertex input layout.
	deviceContext->IASetInputLayout(m_Layout);

    // Set the vertex and pixel shaders that will be used to render the triangles.
    deviceContext->VSSetShader(m_VertexShader, NULL, 0);
    deviceContext->PSSetShader(m_PixelShader, NULL, 0);

	// Set the sampler state in the pixel shader.
	deviceContext->PSSetSamplers(0, 1, &m_SamplerState);

	// Render the triangles.
	deviceContext->DrawIndexed(indexCount, 0, 0);
}