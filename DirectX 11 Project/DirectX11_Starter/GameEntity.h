#ifndef GAMEENTITY_H
#define GAMEENTITY_H

#include <d3d11.h>
#include "Mesh.h"
#include "Material.h"

class GameEntity
{
public:
	GameEntity();
	GameEntity(Mesh*, Material*, DirectX::XMFLOAT3);
	~GameEntity();

	void WorldTransition();
	void Move();
	void Draw(ID3D11DeviceContext*);

#pragma region Gets/Sets

	void SetMesh(Mesh* m_nMesh);
	Mesh* GetMesh();
	__declspec(property(get = GetMesh, put = SetMesh)) Mesh* mesh;

	void SetMaterial(Material*);
	Material* GetMaterial();
	__declspec(property(get = GetMaterial, put = SetMaterial)) Material* material;

	void SetWorldMatrix(DirectX::XMFLOAT4X4 m_nWM);
	DirectX::XMFLOAT4X4& GetWorldMatrix();
	__declspec(property(get = GetWorldMatrix, put = SetWorldMatrix)) DirectX::XMFLOAT4X4 WorldMatrix;

	void SetPosition(DirectX::XMFLOAT3);
	DirectX::XMFLOAT3& GetPosition();
	__declspec(property(get = GetPosition, put = SetPosition)) DirectX::XMFLOAT3 Position;

	void SetRotation(DirectX::XMFLOAT3);
	DirectX::XMFLOAT3& GetRotation();
	__declspec(property(get = GetRotation, put = SetRotation)) DirectX::XMFLOAT3 Rotation;

	void SetScale(DirectX::XMFLOAT3);
	DirectX::XMFLOAT3& GetScale();
	__declspec(property(get = GetScale, put = SetScale)) DirectX::XMFLOAT3 Scale;

#pragma endregion

private:
	Mesh* m_Mesh;
	Material* m_Material;
	DirectX::XMFLOAT4X4 m_WorldMatrix;

	DirectX::XMFLOAT3 m_Position;
	DirectX::XMFLOAT3 m_Rotation;
	DirectX::XMFLOAT3 m_Scale;

	float velocity;

private:
	void Reset();
};

#endif