#ifndef MATERIAL_H
#define MATERIAL_H

#include <d3d11.h>
#include <DirectXMath.h>
#include "dxerr.h"
#include "DirectXTK\Inc\WICTextureLoader.h"
#include "Vertex.h"

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
	Material(ID3D11Device* device, ID3D11DeviceContext* deviceContext, const wchar_t* fileLocationName);

	// The Destructor - currently does nothing
	~Material();

	// Sends the texture information to the Graphics Cards to render on screen
	//		Needs a DirectX11 DeviceContext Pointer/Address
	void Draw(ID3D11DeviceContext* deviceContext);

	void LoadShadersAndInputLayout(ID3D11Device* device, ID3DBlob* vsBlob, ID3DBlob* psBlob, D3D11_INPUT_ELEMENT_DESC* vertexDesc, SIZE_T arraySize);

	void Release();
	D3D11_SAMPLER_DESC SamplerDescription();
	void LoadAConstantBuffer(ID3D11Buffer*);

	#pragma region Gets/Sets

	void SetShaderResourceView(ID3D11ShaderResourceView*);
	ID3D11ShaderResourceView* GetShaderResourceView();
	__declspec(property(get = GetShaderResourceView, put = SetShaderResourceView)) ID3D11ShaderResourceView* r_ShaderResourceView;

	// Get/Set SamplerState
	void SetSamplerState(ID3D11SamplerState*);
	ID3D11SamplerState* GetSamplerState();
	__declspec(property(get = GetSamplerState, put = SetSamplerState)) ID3D11SamplerState* r_SamplerState;

	void SetPixelShader(ID3D11PixelShader*);
	ID3D11PixelShader* GetPixelShader();
	__declspec(property(get = GetPixelShader, put = SetPixelShader)) ID3D11PixelShader* r_PixelShader;

	void SetVertexShader(ID3D11VertexShader*);
	ID3D11VertexShader* GetVertexShader();
	__declspec(property(get = GetVertexShader, put = SetVertexShader)) ID3D11VertexShader* r_VertexShader;

	void SetInputLayer(ID3D11InputLayout*);
	ID3D11InputLayout* GetInputLayout();
	__declspec(property(get = GetInputLayout, put = SetInputLayer)) ID3D11InputLayout* r_InputLayout;

	void SetConstantBuffer(ID3D11Buffer*);
	ID3D11Buffer* GetConstantBuffer();
	__declspec(property(get = GetConstantBuffer, put = SetConstantBuffer)) ID3D11Buffer* r_ConstantBuffer;

	#pragma endregion

private:
	ID3D11ShaderResourceView* m_ShaderResourceView;
	ID3D11SamplerState* m_SamplerState;

	ID3D11PixelShader* m_PixelShader;
	ID3D11VertexShader* m_VertexShader;

	ID3D11InputLayout* m_InputLayout;
	ID3D11Buffer* m_ConstantBuffer;
};

#endif