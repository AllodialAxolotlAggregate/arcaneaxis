#pragma once
#include "GameEntity.h"
#include "Tile.h"

class Artifact
{
public:

	// Constructor : must take a GameEntity and it makes tiles based on which faces are in the mesh
	Artifact(GameEntity* _ge)
	{
		this->m_tileArray = nullptr; 

		// figure out how many tiles we need and where they are

		// create array of tiles with position and radius info
		int num = 100; // for testing purposes
		this->m_tileArray = new Tile[num];
		// allocate space for new Tiles
		for(int i = 0; i < num; i++)
		{
			this->m_tileArray[i] = new Tile(); // except with more info
		}

	}

	// Blank constructor - don't use. I don't even know why I wrote it. 
	Artifact()
	{
		this->m_tileArray = nullptr;
	}

	// Destructor
	~Artifact(void)
	{
		// Delete the tile array regardless of whether or not
		// it exists yet
		if(this->m_tileArray == nullptr)
		{
			delete[] m_tileArray;
		}
		else 
		{
			m_tileArray = nullptr; 
			delete[] m_tileArray;
		}
	}

#pragma region HelperMethods
	// Get position : returns a pointer to a XMFLOAT3
	DirectX::XMFLOAT3& GetPosition()
	{
		return this->m_gameEntity->GetPosition();
	}

	// Rotate:  rotates (passes in to Artifact)
	void Rotate(DirectX::XMFLOAT3 _f3)
	{
		this->m_gameEntity->Rotate(_f3);
	}

	void Draw(ID3D11DeviceContext* _deviceContext)
	{
		this->m_gameEntity->Draw(_deviceContext);  // draws base artifact

		// draw tile boundaries

		// draw tile images


	}

#pragma endregion

private: 
	// pointer to inner game entity
	GameEntity* m_gameEntity; 

	Tile* m_tileArray;
};

