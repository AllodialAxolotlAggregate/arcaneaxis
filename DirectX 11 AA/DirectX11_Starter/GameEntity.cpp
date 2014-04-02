#include "GameEntity.h"

#define IDENTITY_MATRIX XMFLOAT4X4(1.0, 0.0, 0.0, 0.0, 0.0, 1.0, 0.0, 1.0, 0.0, 0.0, 1.0, 0.0, 0.0, 0.0, 0.0, 1.0);

using namespace DirectX;

GameEntity::GameEntity()
{
	Reset();
	WorldTransition();
	velocity = 0.01;
}

GameEntity::GameEntity(Mesh* m_nMesh, Material* m_nMaterial, XMFLOAT3 m_nPosition)
{
	m_Mesh = m_nMesh;
	m_Material = m_nMaterial;

	m_Position = XMFLOAT3(m_nPosition.x, m_nPosition.y, m_nPosition.z);
	m_Rotation = XMFLOAT3(0.0, 0.0, 0.0);
	m_Scale = XMFLOAT3(1.0, 1.0, 1.0);
	WorldTransition();
	velocity = 0.01;
}

GameEntity::~GameEntity() {}

void GameEntity::WorldTransition()
{
	XMMATRIX trans = XMMatrixTranslation(m_Position.x, m_Position.y, m_Position.z);
	XMMATRIX rotX = XMMatrixRotationY(m_Rotation.x);
	XMMATRIX rotY = XMMatrixRotationY(m_Rotation.y);
	XMMATRIX rotZ = XMMatrixRotationY(m_Rotation.z);
	XMMATRIX scale = XMMatrixScaling(m_Scale.x, m_Scale.y, m_Scale.z);

	XMMATRIX w = scale * rotX * rotY * rotZ * trans;

	XMStoreFloat4x4(&m_WorldMatrix, XMMatrixTranspose(w));//*/
}

void GameEntity::Move()
{
	if(m_Position.x <= -.5)
		velocity = 0.01;
	else if(m_Position.x >= .5)
		velocity = -0.01;

	m_Position.x += velocity;

	WorldTransition();
}

void GameEntity::Draw(Camera* aCamera)
{
	material->Draw(aCamera);
	mesh->Draw();
}

void GameEntity::SetMesh(Mesh* m_nMesh) { m_Mesh = m_nMesh; }
Mesh* GameEntity::GetMesh() { return m_Mesh; }

void GameEntity::SetMaterial(Material* m_nMaterial) { m_Material = m_nMaterial; }
Material* GameEntity::GetMaterial() { return m_Material; }

void GameEntity::SetWorldMatrix(XMFLOAT4X4 m_nWM) { m_WorldMatrix = m_nWM; }
XMFLOAT4X4& GameEntity::GetWorldMatrix() { return m_WorldMatrix; }

void GameEntity::SetPosition(XMFLOAT3 m_nPosition) { m_Position = m_nPosition; }
XMFLOAT3& GameEntity::GetPosition() { return m_Position; }

void GameEntity::SetRotation(XMFLOAT3 m_nRotation) { m_Rotation = m_nRotation; }
XMFLOAT3& GameEntity::GetRotation() { return m_Rotation; }

void GameEntity::SetScale(XMFLOAT3 m_nScale) { m_Scale = m_nScale; }
XMFLOAT3& GameEntity::GetScale() { return m_Scale; }

void GameEntity::Reset()
{
	m_Mesh = nullptr;
	m_Material = nullptr;
	m_WorldMatrix = IDENTITY_MATRIX;

	m_Position = XMFLOAT3(0.0, 0.0, 0.0);
	m_Rotation = XMFLOAT3(0.0, 0.0, 0.0);
	m_Scale = XMFLOAT3(1.0, 1.0, 1.0);
}