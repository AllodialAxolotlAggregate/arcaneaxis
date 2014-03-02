#ifndef MATERIAL_H
#define MATERIAL_H

#include <d3d11.h>
#include <DirectXMath.h>
#include "dxerr.h"
#include "DirectXTK\Inc\WICTextureLoader.h"

class Material
{
public:
	Material();

// Material Needs:
//		DirectX11 Device Ponter/Address,
//		DirectX11 DeviceContext Pointer/Address,
//		DirectX11 SamplerState Ponter/Address,
//		the file name specified by its location: L"images/name.png"
	Material(ID3D11Device* device, ID3D11DeviceContext* deviceContext, ID3D11SamplerState* otherSampler, const wchar_t* fileLocationName);
	~Material();

	#pragma region Gets/Sets

	// Get/Set Shader Resource View
	void SetShaderResourceView(ID3D11ShaderResourceView*);
	ID3D11ShaderResourceView* GetShaderResourceView();
	__declspec(property(get = GetShaderResourceView, put = SetShaderResourceView)) ID3D11ShaderResourceView* SRV;

	// Get/Set SamplerState
	void SetSamplerState(ID3D11SamplerState*);
	ID3D11SamplerState* GetSamplerState();
	__declspec(property(get = GetSamplerState, put = SetSamplerState)) ID3D11SamplerState* Sampler;

	#pragma endregion

private:
	ID3D11ShaderResourceView* r_ShaderResourceView;
	ID3D11SamplerState* r_SamplerState;
};

#endif