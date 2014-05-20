#ifndef RYANARTIFACT_H
#define RYANARTIFACT_H

#include <DirectXMath.h>
#include <vector>
#include <sstream>
#include "DXGame.h"
#include "GameEntity.h"
#include "Sentence.h"
#include <random>

//struct SurfaceMaterial
//{
//	std::wstring matName;
//	DirectX::XMFLOAT4 difColor;
//	int texArrayIndex;
//	bool hasTexture;
//	bool transparent;
//};

class RyanArtifact
{
public:
	RyanArtifact();
	RyanArtifact(ID3D11Device* device, ID3D11DeviceContext* deviceContext);
	~RyanArtifact();

	void Draw();
	void LoadStuff(const wchar_t* vsFile, const wchar_t* psFile, D3D11_INPUT_ELEMENT_DESC* vertexDesc, SIZE_T arraySize,ID3D11Buffer* aCSBuffer, VertexShaderConstantBuffer* aConstantBufferData, bool);
	void LoadOBJs();
	void Create(DirectX::XMFLOAT3);
	void Rotate(DirectX::XMFLOAT3);
	void Spin();
	void AddAccel(float x, float y);
	void DontRender();
	void MoveTo(DirectX::XMFLOAT3);

	void LoadObjModel(std::wstring filename,
		Mesh& mesh,
		bool isRHCoordSys);

#pragma region Gets/Sets

	int NumberOfFace() { return m_StoneMesh->r_NumberOfFaces; }

	void SetStone(GameEntity* other) { m_Stone = other; }
	GameEntity* GetStone() { return m_Stone; }
	__declspec(property(get = GetStone, put = SetStone)) GameEntity* r_Stone;

	void SetTiles(GameEntity* other) { m_Tiles = other; }
	GameEntity* GetTiles() { return m_Tiles; }
	__declspec(property(get = GetTiles, put = SetTiles)) GameEntity* r_Tiles;

	GameEntity* GetTile() { return m_Tiles; }

#pragma endregion

private:
	Material* m_StoneMaterial;
	Mesh* m_StoneMesh;
	GameEntity* m_Stone;

	Material* m_RuneMaterial;
	//Mesh* m_RuneMesh;
	//GameEntity* m_Rune;

	DirectX::XMFLOAT3 m_Position;

	// Test Variables
	GameEntity* m_Tiles;
	Mesh* m_ManyMeshes;

	// Physics stuff - for rotation
	float velocX;
	float velocY;
	float accX; 
	float accY;

	// obj verts and inds
	Vertex* objVertices; // array of vertices
	UINT* objListOfIndices; // list of indices
	int objMeshTraingles; // number of triangles

	DirectX::XMMATRIX meshWorld;
	int meshSubsets;
	std::vector<int> meshSubsetIndexStart;
	std::vector<int> meshSubsetTexture;
	//std::vector<SurfaceMaterial> material;
	/*std::vector<ID3D11ShaderResourceView*> meshSRV;
	std::vector<std::wstring> textureNameArray;*/
};

#endif