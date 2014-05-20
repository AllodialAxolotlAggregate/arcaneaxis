#pragma once

#include <DirectXMath.h>
#include <vector>
#include <sstream>
#include "DXGame.h"
#include "GameEntity.h"
#include "Artifact.h"
#include "Text.h"
#include "Sentence.h"
#include "GameManager.h"

// Include run-time memory checking in debug builds
#if defined(DEBUG) || defined(_DEBUG)
#define _CRTDBG_MAP_ALLOC
#include <crtdbg.h>
#endif

// For DirectX Math
using namespace DirectX;

struct SurfaceMaterial
{
	std::wstring matName;
	XMFLOAT4 difColor;
	int texArrayIndex;
	bool hasTexture;
	bool transparent;
};

// Demo class which extends the base DXGame class
class DemoGame : public DXGame
{
public:
	DemoGame(HINSTANCE hInstance);
	~DemoGame();

	// Overrides for base level methods
	bool Init();
	void OnResize();
	void UpdateScene(float dt);
	void DrawScene(); 

	// For handing mouse input
	void OnMouseDown(WPARAM btnState, int x, int y);
	void OnMouseUp(WPARAM btnState, int x, int y);
	void OnMouseMove(WPARAM btnState, int x, int y);

	void Keyboard();
	void Draw2D();
	bool PointInFace(Face* _f);

private:
	// Initialization for our "game" demo
	void CreateGeometryBuffers();
	void LoadShadersAndInputLayout();
	void Release();

	//Define LoadObjModel function after we create surfaceMaterial structure
	void LoadObjModel(std::wstring filename,		//.obj filename
		Material& material,							//vector of material structures
		Mesh& mesh,									//mesh
		bool isRHCoordSys);							//true if model was created in right hand coord system

	bool MouseIsOverEntity(GameEntity* e);
private:
	// A few more odds and ends we'll need
	ID3D11Buffer* vsConstantBuffer;
	VertexShaderConstantBuffer vsConstantBufferData;

	// Keeps track of the old mouse position.  Useful for 
	// determining how far the mouse moved in a single frame.
	POINT cursorPos; // gmb9280 testing mouse coords
	POINT windowOffset;
	bool mouseDragging;
	POINT dragStarted; 
	// My Extra Stuff
	GameEntity* ges;
	Material* ma;
	Mesh* mish;
	float time;

	Font* font;
	FontShader* fShader;
	Sentence* sentence;

	// Bob's Obj Globals
	//ID3D11BlendState* Transparency;
	GameEntity obj;
	GameEntity obj1;
	std::vector<GameEntity> entities;

	Material* maSphere;
	Mesh* meSphere;
	GameEntity* okamaGameSphere;

	//gmb9280: added Artifact
	Artifact* gameArtifact;

	// obj verts and inds
	Vertex* objVertices; // array of vertices
	UINT* objListOfIndices; // list of indices
	int objMeshTraingles; // number of triangles


	// Stuff for skybox
	Mesh* skyboxMesh;
	Material* skyboxMaterial;
	GameEntity* skybox;

	ID3D11Buffer* meshVertBuff;
	ID3D11Buffer* meshIndexBuff;
	XMMATRIX meshWorld;
	int meshSubsets;
	std::vector<int> meshSubsetIndexStart;
	std::vector<int> meshSubsetTexture;
	std::vector<SurfaceMaterial> material;
	std::vector<ID3D11ShaderResourceView*> meshSRV;
	std::vector<std::wstring> textureNameArray;

	XMMATRIX WVP;
	XMMATRIX World;

	// Obj1 Globals
	ID3D11Buffer* meshVertBuff1;
	ID3D11Buffer* meshIndexBuff1;


	// Rotate Globals
	bool artifactTurnLeft;
	bool artifactTurnRight;
	float artifactTurnRateX;

	// Lighting
	ID3D11Buffer* cbPerFrameBuffer;
	cbPerFrame constbuffPerFrame;
	Light light;
	XMVECTOR lightVector;

	GameManager* manager;

	// Menu bools
	bool pausePressed;

	// Mouse World Coords
	float mouseWorldX;
	float mouseWorldY;

};