#include "GameEntity.h"

#define IDENTITY_MATRIX XMFLOAT4X4(1.0, 0.0, 0.0, 0.0, 0.0, 1.0, 0.0, 1.0, 0.0, 0.0, 1.0, 0.0, 0.0, 0.0, 0.0, 1.0);

using namespace DirectX;

GameEntity::GameEntity()
{
	Reset();
	WorldTransition();
	velocity = 0.01;
}

GameEntity::GameEntity(Mesh* m_nMesh, Material* m_nMaterial, XMFLOAT3 m_nPosition) :
	m_Mesh(m_nMesh),
	m_Material(m_nMaterial),
	m_Position(m_nPosition),
	m_Rotation(XMFLOAT3(0.0, 0.0, 0.0)),
	m_Scale(XMFLOAT3(1.0, 1.0, 1.0)),
	velocity(0.01)
{
	WorldTransition();
}

GameEntity::~GameEntity() {}

void GameEntity::WorldTransition()
{
	XMMATRIX trans = XMMatrixTranslation(m_Position.x, m_Position.y, m_Position.z);
	XMMATRIX rotX = XMMatrixRotationX(m_Rotation.x);
	XMMATRIX rotY = XMMatrixRotationY(m_Rotation.y);
	XMMATRIX rotZ = XMMatrixRotationZ(m_Rotation.z);
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

void GameEntity::MoveTo(XMFLOAT3 tran)
{
	m_Position.x = tran.x;
	m_Position.y = tran.y;
	m_Position.z = tran.z;

	WorldTransition();
}

void GameEntity::Rotate(XMFLOAT3 rot)
{
	m_Rotation.x += rot.x;
	m_Rotation.y += rot.y;
	m_Rotation.z += rot.z;

	WorldTransition();
}

void GameEntity::Draw()
{
	material->Draw(m_WorldMatrix);
	mesh->Draw();
}

void GameEntity::Reset()
{
	m_Mesh = nullptr;
	m_Material = nullptr;
	m_WorldMatrix = IDENTITY_MATRIX;

	m_Position = XMFLOAT3(0.0, 0.0, 0.0);
	m_Rotation = XMFLOAT3(0.0, 0.0, 0.0);
	m_Scale = XMFLOAT3(1.0, 1.0, 1.0);
}