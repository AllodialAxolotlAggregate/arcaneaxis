#ifndef MATERIAL_H
#define MATERIAL_H

#include <d3d11.h>
#include <DirectXMath.h>
#include <d3dcompiler.h>
#include "dxerr.h"
#include "DirectXTK\Inc\WICTextureLoader.h"
#include "Vertex.h"
#include "Camera.h"

class Material
{
public:
	// An empty Constructor that sets everything to nullptr
	Material();

	// Material class holds image information to be rendered on screen
	// Needs:
	//		DirectX11 Device Ponter/Address,
	//		DirectX11 DeviceContext Pointer/Address,
	//		the file name specified by its location: L"images/name.png"
	Material(ID3D11Device* device, ID3D11DeviceContext* deviceContext);

	// The Destructor - currently does nothing
	~Material();

	// Returns a sampler description
	D3D11_SAMPLER_DESC SamplerDescription();

	// Sends the texture information to the Graphics Cards to render on screen
	//		Needs a DirectX11 DeviceContext Pointer/Address
	void Draw(DirectX::XMFLOAT4X4);

	//void LoadShadersAndInputLayout(ID3DBlob* vsBlob, ID3DBlob* psBlob, D3D11_INPUT_ELEMENT_DESC* vertexDesc, SIZE_T arraySize);
	void LoadShadersAndInputLayout(const wchar_t* vsFile, const wchar_t* psFile, D3D11_INPUT_ELEMENT_DESC* vertexDesc, SIZE_T arraySize);
	void LoadSamplerStateAndShaderResourceView(const wchar_t* fileLocationName);

	void Release();
	void LoadAConstantBuffer(ID3D11Buffer* aConstantBuffer, VertexShaderConstantBuffer* aConstantBufferData);

	void SetUpDraw();
	void ActuallyDraw(DirectX::XMFLOAT4X4);

	#pragma region Gets/Sets

	ID3D11ShaderResourceView* GetShaderResourceView() { return m_ShaderResourceView; }
	void SetShaderResourceView(ID3D11ShaderResourceView* oSRV) { m_ShaderResourceView = oSRV; }
	__declspec(property(get = GetShaderResourceView, put = SetShaderResourceView)) ID3D11ShaderResourceView* r_ShaderResourceView;

	// Get/Set SamplerState
	ID3D11SamplerState* GetSamplerState() { return m_SamplerState; }
	void SetSamplerState(ID3D11SamplerState* oSampler) { m_SamplerState = oSampler; }
	__declspec(property(get = GetSamplerState, put = SetSamplerState)) ID3D11SamplerState* r_SamplerState;

	ID3D11PixelShader* GetPixelShader() { return m_PixelShader; }
	void SetPixelShader(ID3D11PixelShader* otherPixelShader) { m_PixelShader = otherPixelShader; }
	__declspec(property(get = GetPixelShader, put = SetPixelShader)) ID3D11PixelShader* r_PixelShader;

	ID3D11VertexShader* GetVertexShader() { return m_VertexShader; }
	void SetVertexShader(ID3D11VertexShader* otherVertexShader) { m_VertexShader = otherVertexShader; }
	__declspec(property(get = GetVertexShader, put = SetVertexShader)) ID3D11VertexShader* r_VertexShader;

	ID3D11InputLayout* GetInputLayout() { return m_InputLayout; }
	void SetInputLayer(ID3D11InputLayout* otherInputLayout) { m_InputLayout = otherInputLayout; }
	__declspec(property(get = GetInputLayout, put = SetInputLayer)) ID3D11InputLayout* r_InputLayout;

	ID3D11Buffer* GetConstantBuffer() { return m_ConstantBuffer; }
	void SetConstantBuffer(ID3D11Buffer* otherConstantBuffer) { m_ConstantBuffer = otherConstantBuffer; }
	__declspec(property(get = GetConstantBuffer, put = SetConstantBuffer)) ID3D11Buffer* r_ConstantBuffer;

	VertexShaderConstantBuffer* GetContantBufferData() { return m_ConstantBufferData; }
	void SetConstantBufferData(VertexShaderConstantBuffer* otherConstantBufferData) { m_ConstantBufferData = otherConstantBufferData; }
	__declspec(property(get = GetContantBufferData, put = SetConstantBufferData)) VertexShaderConstantBuffer* r_ConstantBufferData;

	ID3D11Device* GetDevice() { return m_Device; }
	void SetDevice(ID3D11Device* otherDevice) { m_Device = otherDevice; }
	__declspec(property(get = GetDevice, put = SetDevice)) ID3D11Device* r_Device;

	ID3D11DeviceContext* GetDeviceContext() { return m_DeviceContext; }
	void SetDeviceContext(ID3D11DeviceContext* otherDeviceContext) { m_DeviceContext = otherDeviceContext; }	__declspec(property(get = GetDeviceContext, put = SetDeviceContext)) ID3D11DeviceContext* r_DeviceContext;

	#pragma endregion

private:
	ID3D11ShaderResourceView* m_ShaderResourceView;
	ID3D11SamplerState* m_SamplerState;

	ID3D11PixelShader* m_PixelShader;
	ID3D11VertexShader* m_VertexShader;

	ID3D11InputLayout* m_InputLayout;
	ID3D11Buffer* m_ConstantBuffer;
	VertexShaderConstantBuffer* m_ConstantBufferData;

	ID3D11Device* m_Device;
	ID3D11DeviceContext* m_DeviceContext;
};

#endif