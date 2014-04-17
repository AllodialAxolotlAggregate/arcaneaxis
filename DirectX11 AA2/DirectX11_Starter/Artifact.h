#pragma once
#include "GameEntity.h"
class Artifact
{
public:

	// Constructor : must take a GameEntity and it makes tiles based on which faces are in the mesh
	Artifact(GameEntity* _ge)
	{

	}

	// Destructor
	~Artifact(void);

#pragma region HelperMethods
	// Get position : returns a pointer to a XMFLOAT3
	DirectX::XMFLOAT3& GetPosition()
	{
		return this->m_gameEntity->GetPosition();
	}

	//

#pragma endregion

private: 
	// pointer to inner game entity
	GameEntity* m_gameEntity; 

};

