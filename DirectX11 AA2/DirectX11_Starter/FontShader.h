#ifndef FONTSHADER_H
#define FONTSHADER_H

#include <d3d11.h>
#include <DirectXMath.h>
#include "dxerr.h"
#include <d3dcompiler.h>
#include "DirectXTK\Inc\WICTextureLoader.h"
#include "Vertex.h"

class FontShader
{
public:
	FontShader();
	~FontShader();

	void Shutdown();
	void Initialize(ID3D11Device* device, WCHAR* vsFilename, WCHAR* psFilename);
	void Render(ID3D11DeviceContext* deviceContext, int indexCount, DirectX::XMFLOAT4X4 worldMatrix, DirectX::XMFLOAT4X4 viewMatrix, 
							 DirectX::XMFLOAT4X4 projectionMatrix, ID3D11ShaderResourceView* texture, DirectX::XMFLOAT4 pixelColor);

	void LoadAConstantBuffer(ID3D11Buffer* aCSBuffer) { m_ConstantBuffer = aCSBuffer; }


#pragma region Gets/Sets

	ID3D11VertexShader* GetVertexShader() { return m_VertexShader; }
	void SetVertexShader(ID3D11VertexShader* otherVertexShader) { m_VertexShader = otherVertexShader; }
	__declspec(property(get = GetVertexShader, put = SetVertexShader)) ID3D11VertexShader* r_VertexShader;

	ID3D11PixelShader* GetPixelShader() { return m_PixelShader; }
	void SetPixelShader(ID3D11PixelShader* otherPixelShader) { m_PixelShader = otherPixelShader; }
	__declspec(property(get = GetPixelShader, put = SetPixelShader)) ID3D11PixelShader* r_PixelShader;

	ID3D11InputLayout* GetInputLayout() { return m_Layout; }
	void SetInputLayout (ID3D11InputLayout* otherLayout) { m_Layout = otherLayout; }
	__declspec(property(get = GetInputLayout, put = SetInputLayout)) ID3D11InputLayout* r_InputLayout;

	ID3D11Buffer* GetConstantBuffer() { return m_ConstantBuffer; }
	void SetConstantBuffer(ID3D11Buffer* otherConstant) { m_ConstantBuffer = otherConstant; }
	__declspec(property(get = GetConstantBuffer, put = SetConstantBuffer)) ID3D11Buffer* r_ConstantBuffer;

	ID3D11SamplerState* GetSamplerState() { return m_SamplerState; }
	void SetSamplerState(ID3D11SamplerState* otherSamplerState) { m_SamplerState = otherSamplerState; }
	__declspec(property(get = GetSamplerState, put = SetSamplerState)) ID3D11SamplerState* r_SamplerState;

	ID3D11Buffer* GetPixelBuffer() { return m_PixelBuffer; }
	void SetPixelBuffer(ID3D11Buffer* otherPixelBuffer) { m_PixelBuffer = otherPixelBuffer; }
	__declspec(property(get = GetPixelBuffer, put = SetPixelBuffer)) ID3D11Buffer* r_PixelBuffer;

#pragma endregion

private:
	void SetShaderParameters(ID3D11DeviceContext* deviceContext, DirectX::XMFLOAT4X4 worldMatrix, DirectX::XMFLOAT4X4 viewMatrix, 
										  DirectX::XMFLOAT4X4 projectionMatrix, ID3D11ShaderResourceView* texture, DirectX::XMFLOAT4 pixelColor);
	void RenderShader(ID3D11DeviceContext* deviceContext, int indexCount);

private:
	ID3D11VertexShader* m_VertexShader;
	ID3D11PixelShader* m_PixelShader;

	ID3D11SamplerState* m_SamplerState;

	ID3D11InputLayout* m_Layout;
	ID3D11Buffer* m_ConstantBuffer;

	ID3D11Buffer* m_PixelBuffer;
};

#endif