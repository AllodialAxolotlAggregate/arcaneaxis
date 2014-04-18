#ifndef SENTENCE_H
#define SENTENCE_H

#include "Font.h"
#include "FontShader.h"

class Sentence
{
public:
	Sentence();
	Sentence(ID3D11Device* device, ID3D11DeviceContext* deviceContext);
	~Sentence();

	void LoadFontAndShader(Font* aFont, FontShader* aFontShader);
	void Initialize(char* text, int positionX, int positionY);
	void Render(DirectX::XMFLOAT4X4 viewMatrix, DirectX::XMFLOAT4X4 projectionMatrix);
	void Release();
	void WorldTransition();

#pragma region Gets/Sets

	void SetPosition(DirectX::XMFLOAT3 otherPosition) { m_Position = otherPosition; }
	DirectX::XMFLOAT3& GetPosition() { return m_Position; }
	__declspec(property(get = GetPosition, put = SetPosition)) DirectX::XMFLOAT3 r_Position;

	void SetWorldMatrix(DirectX::XMFLOAT4X4 m_nWM) { m_WorldMatrix = m_nWM; }
	DirectX::XMFLOAT4X4& GetWorldMatrix() { return m_WorldMatrix; }
	__declspec(property(get = GetWorldMatrix, put = SetWorldMatrix)) DirectX::XMFLOAT4X4 WorldMatrix;

#pragma endregion

private:
	Font* m_Font;
	FontShader* m_FontShader;

	ID3D11Buffer *m_VertexBuffer, *m_IndexBuffer;
	int m_VertexCount, m_IndexCount, m_Length;
	DirectX::XMFLOAT4 m_Color;
	DirectX::XMFLOAT3 m_Position;
	DirectX::XMFLOAT4X4 m_WorldMatrix;

	ID3D11Device* m_Device;
	ID3D11DeviceContext* m_DeviceContext;
};

#endif