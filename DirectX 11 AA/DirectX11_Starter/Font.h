#ifndef FONT_H
#define FONT_H

#include <d3d11.h>
#include <DirectXMath.h>
#include "dxerr.h"
#include "Vertex.h"
#include <fstream>
#include "DirectXTK\Inc\WICTextureLoader.h"

// Based off of RasterTek tutorial 12:Font Engine
// http://www.rastertek.com/dx11tut12.html
class Font
{
public:
	Font();
	~Font();

	//void Initialize(char*);
	void Initialize(ID3D11Device* device,ID3D11DeviceContext* deviceContext, char* fontFilename, WCHAR* textureFilename);
	void Shutdown();
	void BuildVertexArray(void* vertices, char* sentence, float drawX, float drawY);

#pragma region Get/Set

	FontType* GetFont() { return m_Font; }
	void SetFont(FontType* otherFont) { m_Font = otherFont; }
	__declspec(property(get = GetFont, put = SetFont)) FontType* r_Font;

	ID3D11ShaderResourceView* GetShaderResourceView() { return m_ShaderResourceView; }
	void SetShaderResourceView(ID3D11ShaderResourceView* otherSRV) { m_ShaderResourceView = otherSRV; }
	__declspec(property(get = GetShaderResourceView, put = SetShaderResourceView)) ID3D11ShaderResourceView* r_ShaderResourceView;

#pragma endregion

private:
	void LoadFontData(char*);
	void ReleaseFontData();
	void ReleaseTexture();

private:
	FontType* m_Font;
	ID3D11ShaderResourceView* m_ShaderResourceView;
};

#endif