#pragma once
#include "GameEntity.h"
#include "Tile.h"
#include <iostream>

class Artifact
{
public:

	// Constructor : must take a GameEntity and it makes tiles based on which faces are in the mesh
	Artifact(GameEntity* _ge)
	{
		this->m_tileArray = nullptr; 
		this->m_gameEntity = _ge;
		// figure out how many tiles we need and where they are

		// generate tiles based on 
		// the verts of the mesh contained		
		// (Do this after these values are initialized....)
	}

	// Blank constructor - don't use. I don't even know why I wrote it. ¯\_(^u^)_/¯
	Artifact()
	{
		this->m_tileArray = nullptr;
	}

	// Generates tiles
	void GenTiles(Vertex* objVerts, UINT* listInds, int numTris)
	{
		//Figure out how many verts we need
		//int numVerts = this->m_gameEntity->GetNumVertsStraightUp();

		// every three verts makes a tile (triangles)
		this->m_tileArray = new Tile*(); // new array of tile pointers

		for(int i = 0; i< numTris-1; i++)
		{
			//std::cout<< "Triangle: " << i << std::endl;

			Vertex* v1 = &objVerts[listInds[(i*3)]];
			Vertex* v2 = &objVerts[listInds[(i*3)+1]];
			Vertex* v3 = &objVerts[listInds[(i*3)+2]];
			this->m_tileArray[i] = new Tile( v1, v2, v3);
		}
		// done with generating tiles

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
			delete[] m_tileArray;
		}
	}

#pragma region HelperMethods


	// get the game entity
	GameEntity* getGameEntity()
	{
		if(this->m_gameEntity != nullptr)
		{
			return this->m_gameEntity;
		}
		else
		{
			return nullptr;
		}
	}


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


#pragma endregion

private: 
	// pointer to inner game entity
	GameEntity* m_gameEntity; 

	Tile** m_tileArray; // array of pointers to tiles

	
};

