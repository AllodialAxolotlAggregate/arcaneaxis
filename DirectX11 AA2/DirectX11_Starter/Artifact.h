#pragma once
#include "GameEntity.h"
#include "Tile.h"
#include <iostream>

using namespace DirectX;

class Artifact
{
public:

	// Constructor : must take a GameEntity and it makes tiles based on which faces are in the mesh
	Artifact(GameEntity* _ge)
	{
		this->m_tileArray = nullptr; 
		this->m_gameEntity = _ge;

		// Ryan's Stuff
		// Write down the number of faces on object
		m_NumberOfFaces = m_gameEntity->mesh->r_NumberOfFaces;
		// Create that many tiles GameEntities
		m_Tiles = new GameEntity[m_NumberOfFaces];

		// Create a single material for all of the entities to use
		m_SingleMaterial = new Material(m_gameEntity->mesh->r_Device, m_gameEntity->mesh->r_DeviceContext);
		m_SingleMaterial->LoadSamplerStateAndShaderResourceView(L"Ignite.jpg");

		// Create as many independent meshes as there are faces
		m_ManyMeshes = new Mesh[m_NumberOfFaces];

		for(int i = 0; i < m_NumberOfFaces; ++i)
		{
			// Offload the meshes to a localized array of vertices
			Vertex vertices[] = 
			{
				{ m_gameEntity->mesh->r_Faces[i].r_Vertices[0].Position, m_gameEntity->mesh->r_Faces[i].r_Vertices[0].Color, XMFLOAT2(0.0, 0.0), m_gameEntity->mesh->r_Faces[i].r_Vertices[0].Normal },
				{ m_gameEntity->mesh->r_Faces[i].r_Vertices[1].Position, m_gameEntity->mesh->r_Faces[i].r_Vertices[1].Color, XMFLOAT2(0.5, 0.5), m_gameEntity->mesh->r_Faces[i].r_Vertices[1].Normal},
				{ m_gameEntity->mesh->r_Faces[i].r_Vertices[2].Position, m_gameEntity->mesh->r_Faces[i].r_Vertices[2].Color, XMFLOAT2(1.0, 1.0), m_gameEntity->mesh->r_Faces[i].r_Vertices[2].Normal}
			};

			// add to the array of meshes
			m_ManyMeshes[i] = Mesh(m_gameEntity->mesh->r_Device, m_gameEntity->mesh->r_DeviceContext);
			m_ManyMeshes[i].LoadNumbers(3, 3);
			m_ManyMeshes[i].LoadBuffers(vertices, m_gameEntity->mesh->r_Faces->r_Indices);

			// load the GameEntity
			m_Tiles[i] = GameEntity(&m_ManyMeshes[i], m_SingleMaterial, m_gameEntity->Position);
		}
	}

	// Blank constructor - don't use. I don't even know why I wrote it. �\_(^u^)_/�
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


			
			Vertex* v1 = new Vertex(objVerts[listInds[(i*3)]]);
			Vertex* v2 = new Vertex(objVerts[listInds[(i*3)+1]]);
			Vertex* v3 = new Vertex(objVerts[listInds[(i*3)+2]]);
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

		if(m_Tiles != nullptr)
		{
			delete[] m_Tiles;
			m_Tiles = nullptr;
		}

		if(m_SingleMaterial != nullptr)
		{
			delete m_SingleMaterial;
			m_SingleMaterial = nullptr;
		}

		if(m_ManyMeshes != nullptr)
		{
			delete[] m_ManyMeshes;
			m_ManyMeshes = nullptr;
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

	void Draw()
	{
		//m_gameEntity->Draw();
		for(int i = 0; i < m_NumberOfFaces; ++i)
			m_Tiles[i].Draw();
	}

	void LoadStuff(const wchar_t* vsFile, const wchar_t* psFile, D3D11_INPUT_ELEMENT_DESC* vertexDesc, SIZE_T arraySize,ID3D11Buffer* aCSBuffer, VertexShaderConstantBuffer* aConstantBufferData)
	{
		for(int i = 0; i < m_NumberOfFaces; ++i)
		{
			m_Tiles[i].material->LoadShadersAndInputLayout(vsFile, psFile, vertexDesc, arraySize);
			m_Tiles[i].material->LoadAConstantBuffer(aCSBuffer, aConstantBufferData);
		}
	}

private: 
	// pointer to inner game entity
	GameEntity* m_gameEntity; 
	Tile** m_tileArray; // array of pointers to tiles

	// Ryan's Stuff
	GameEntity* m_Tiles;
	Material* m_SingleMaterial;
	Mesh* m_ManyMeshes;
	int m_NumberOfFaces;
};

