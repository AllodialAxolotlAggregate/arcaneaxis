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
		this->m_gameEntity = _ge;
		// figure out how many tiles we need and where they are

		this->GenTiles( this->m_gameEntity->GetVerticesStraightUp() ); // generate tiles based on 
		// the verts of the mesh contained
	}

	// Blank constructor - don't use. I don't even know why I wrote it. ¯\_(^u^)_/¯
	Artifact()
	{
		this->m_tileArray = nullptr;
	}

	// Generates tiles
	void GenTiles(Vertex** _verts)
	{
		//Figure out how many verts we need
		int numVerts = this->m_gameEntity->GetNumVertsStraightUp();

		// every three verts makes a tile (triangles)


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
		//this->m_gameEntity->Draw(_deviceContext);  // draws base artifact

		// draw tile boundaries

		// draw tile images
	}



#pragma endregion

private: 
	// pointer to inner game entity
	GameEntity* m_gameEntity; 

	Tile** m_tileArray; // array of pointers to tiles

	
};

