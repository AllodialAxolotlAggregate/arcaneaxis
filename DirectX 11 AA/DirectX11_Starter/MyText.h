#ifndef MYTEXT_H
#define MYTEXT_H

#include <d3d11.h>
#include "Mesh.h"
#include "Material.h"
#include "Camera.h"

class MyText
{
public:
	MyText();
	~MyText();

	void WorldTransition();
	void Draw(Camera* aCamera);

	void BuildVertexArray(void* vertices, char* sentence, float drawX, float drawY);

#pragma region Gets/Sets

	void SetMesh(Mesh* m_nMesh) { m_Mesh = m_nMesh; }
	Mesh* GetMesh() { return m_Mesh; }
	__declspec(property(get = GetMesh, put = SetMesh)) Mesh* mesh;

	void SetMaterial(Material* otherMaterial) { m_Material = otherMaterial; }
	Material* GetMaterial() { return m_Material; }
	__declspec(property(get = GetMaterial, put = SetMaterial)) Material* material;

	void SetWorldMatrix(DirectX::XMFLOAT4X4 m_nWM) { m_WorldMatrix = m_nWM; }
	DirectX::XMFLOAT4X4& GetWorldMatrix() { return m_WorldMatrix; }
	__declspec(property(get = GetWorldMatrix, put = SetWorldMatrix)) DirectX::XMFLOAT4X4 WorldMatrix;

	void SetPosition(DirectX::XMFLOAT3 otherPosition) { m_Position = otherPosition; }
	DirectX::XMFLOAT3& GetPosition() { return m_Position;}
	__declspec(property(get = GetPosition, put = SetPosition)) DirectX::XMFLOAT3 Position;

#pragma endregion

private:
	Mesh* m_Mesh;
	Material* m_Material;
	FontType* m_Font;
	DirectX::XMFLOAT4X4 m_WorldMatrix;
	DirectX::XMFLOAT3 m_Position;
};

#endif