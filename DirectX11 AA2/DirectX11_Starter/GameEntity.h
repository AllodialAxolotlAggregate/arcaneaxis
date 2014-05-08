#ifndef GAMEENTITY_H
#define GAMEENTITY_H

#include <d3d11.h>
#include "Mesh.h"
#include "Material.h"

class GameEntity
{
public:
	// Empty Constructor
	GameEntity();

	// GameEntity class is an object that contains a mesh and a material
	// to then be used to be put on the screen
	// Needs:
	//		A Mesh Pointer/Address,
	//		A Material Pointer/Address,
	//		A DirectX::XMFlOAT3 position
	GameEntity(Mesh* aMesh, Material* aMaterial, DirectX::XMFLOAT3 aPosition);

	// Empty Deconstructor - currently does nothing
	~GameEntity();

	// Translates the object's vector information into matrix information that
	// the graphic cards uses to pixelate the location
	void WorldTransition();

	// A simple move method that moves the object back and forth
	void Move();

	// Moves the entity to the provided coordinates
	void MoveTo(DirectX::XMFLOAT3);

	// Rotates the entity by the specified amount
	void Rotate(DirectX::XMFLOAT3);

	// Calls the Material's and Mesh's draws methods to render object on the screen
	void Draw();
	//void DrawEntity(Camera*);

#pragma region Gets/Sets

	void SetMesh(Mesh* m_nMesh) { m_Mesh = m_nMesh; }
	Mesh* GetMesh() { return m_Mesh; }
	__declspec(property(get = GetMesh, put = SetMesh)) Mesh* mesh;

	void SetMaterial(Material* m_nMaterial) { m_Material = m_nMaterial; }
	Material* GetMaterial() { return m_Material; }
	__declspec(property(get = GetMaterial, put = SetMaterial)) Material* material;

	void SetWorldMatrix(DirectX::XMFLOAT4X4 m_nWM) { m_WorldMatrix = m_nWM; }
	DirectX::XMFLOAT4X4& GetWorldMatrix() { return m_WorldMatrix; }
	__declspec(property(get = GetWorldMatrix, put = SetWorldMatrix)) DirectX::XMFLOAT4X4 WorldMatrix;

	void SetPosition(DirectX::XMFLOAT3 m_nPosition) { m_Position = m_nPosition; }
	DirectX::XMFLOAT3& GetPosition() { return m_Position; }
	__declspec(property(get = GetPosition, put = SetPosition)) DirectX::XMFLOAT3 Position;

	void SetRotation(DirectX::XMFLOAT3 m_nRotation) { m_Rotation = m_nRotation; }
	DirectX::XMFLOAT3& GetRotation() { return m_Rotation; }
	__declspec(property(get = GetRotation, put = SetRotation)) DirectX::XMFLOAT3 Rotation;

	void SetScale(DirectX::XMFLOAT3 m_nScale) { m_Scale = m_nScale; }
	DirectX::XMFLOAT3& GetScale() { return m_Scale; }
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
	// Resets the values to zero
	void Reset();
};

#endif