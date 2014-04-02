#ifndef TEXT_H
#define TEXT_H

#include "Font.h"
#include "FontShader.h"

class Text
{
public:
	Text();
	~Text();

	void Initialize(ID3D11Device* device, ID3D11DeviceContext* deviceContext, int screenWidth, int screenHeight, DirectX::XMFLOAT4X4 baseViewMatrix);
	void Shutdown();
	void Render(ID3D11DeviceContext* deviceContext, DirectX::XMFLOAT4X4 worldMatrix, DirectX::XMFLOAT4X4 projectionMatrix, DirectX::XMFLOAT4X4 orthoMatrix);

#pragma region Gets/Sets

	Font* GetFont() { return m_Font; }
	void SetFont(Font* otherFont) { m_Font = otherFont; }
	__declspec(property(get = GetFont, put = SetFont)) Font* r_Font;

	FontShader* GetFontShader() { return m_FontShader; }
	void SetFontShader(FontShader* otherFS) { m_FontShader = otherFS; }
	__declspec(property(get = GetFontShader, put = SetFontShader)) FontShader* r_FontShader;

	DirectX::XMFLOAT4X4 GetBaseViewMatrix() { return m_baseViewMatrix; }
	void SetBaseViewMatrix(DirectX::XMFLOAT4X4 otherMatrix) { m_baseViewMatrix = otherMatrix; }
	__declspec(property(get = GetBaseViewMatrix, put = SetBaseViewMatrix)) DirectX::XMFLOAT4X4 r_BaseViewMatrix;

	int GetScreenWidth() { return m_ScreenWidth; }
	void SetScreenWidth(int otherWidth) { m_ScreenWidth = otherWidth; }
	__declspec(property(get = GetScreenWidth, put = SetScreenWidth)) int r_ScreenWidth;

	int GetScreenHeight() { return m_ScreenHeight; }
	void SetScreenHeight(int otherHeight) { m_ScreenHeight = otherHeight; }
	__declspec(property(get = GetScreenHeight, put = SetScreenHeight)) int r_ScreenHeight;

	DirectX::XMFLOAT2 GetPosition() { return m_Position; }
	void SetPosition(DirectX::XMFLOAT2 pos) { m_Position = pos; }
	__declspec(property(get = GetPosition, put = SetPosition)) DirectX::XMFLOAT2 r_Position;

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
	DirectX::XMFLOAT2 m_Position;
	DirectX::XMFLOAT4X4 m_baseViewMatrix;
	SentenceType* m_sentence1;
	SentenceType* m_sentence2;
};

#endif