#ifndef TEXT_H
#define TEXT_H

#include "Font.h"
#include "FontShader.h"

class Text
{
public:
	Text();
	~Text();

	void Initialize(ID3D11Device* device, ID3D11DeviceContext* deviceContext, int screenWidth, int screenHeight);
	void Shutdown();
	void Render(ID3D11DeviceContext* deviceContext, DirectX::XMFLOAT4X4 worldMatrix, DirectX::XMFLOAT4X4 projectionMatrix, DirectX::XMFLOAT4X4 orthoMatrix);

#pragma region Gets/Sets

	Font* GetFont() { return m_Font; }
	void SetFont(Font* otherFont) { m_Font = otherFont; }
	__declspec(property(get = GetFont, put = SetFont)) Font* r_Font;

	FontShader* GetFontShader() { return m_FontShader; }
	void SetFontShader(FontShader* otherFS) { m_FontShader = otherFS; }
	__declspec(property(get = GetFontShader, put = SetFontShader)) FontShader* r_FontShader;

	int GetScreenWidth() { return m_ScreenWidth; }
	void SetScreenWidth(int otherWidth) { m_ScreenWidth = otherWidth; }
	__declspec(property(get = GetScreenWidth, put = SetScreenWidth)) int r_ScreenWidth;

	int GetScreenHeight() { return m_ScreenHeight; }
	void SetScreenHeight(int otherHeight) { m_ScreenHeight = otherHeight; }
	__declspec(property(get = GetScreenHeight, put = SetScreenHeight)) int r_ScreenHeight;

#pragma endregion

private:
	void InitializeSentence(SentenceType** sentence, int maxLength, ID3D11Device* device);
	void UpdateSentence(SentenceType* sentence, char* text, int positionX, int positionY, float red, float green, float blue, ID3D11DeviceContext* deviceContext);
	void ReleaseSentence(SentenceType** sentence);
	void RenderSentence(ID3D11DeviceContext* deviceContext, SentenceType* sentence, DirectX::XMFLOAT4X4 worldMatrix, DirectX::XMFLOAT4X4 projectionMatrix, DirectX::XMFLOAT4X4 orthoMatrix);

private:
	Font* m_Font;
	FontShader* m_FontShader;

	int m_ScreenWidth, m_ScreenHeight;
	SentenceType* m_sentence1;
	SentenceType* m_sentence2;
};

#endif