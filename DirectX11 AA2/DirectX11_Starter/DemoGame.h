#pragma once

#include <DirectXMath.h>
#include "DXGame.h"
#include "Mesh.h"
#include "GameEntity.h"
#include "Text.h"
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

/*ID3D11Buffer cbPerObject
{
	XMFLOAT4 WVP;
    XMFLOAT4 World;

	XMFLOAT4 difColor;
	bool hasTexture;
};*/

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
	void Draw2D(GameEntity&);

private:
	// Initialization for our "game" demo
	//bool LoadObjModel(ID3D11Device*, std::wstring, GameEntity*);
	void CreateGeometryBuffers();
	void LoadShadersAndInputLayout();
	void DrawEntity(GameEntity&);
	void DrawObj(GameEntity&);
	void Release();

	//Define LoadObjModel function after we create surfaceMaterial structure
	void LoadObjModel(ID3D11Device* device,		// device
		ID3D11DeviceContext* deviceContext,		// deviceContext
		std::wstring filename,		//.obj filename
		ID3D11Buffer** vertBuff,			//mesh vertex buffer
		ID3D11Buffer** indexBuff,			//mesh index buffer
		std::vector<int>& subsetIndexStart,		//start index of each subset
		std::vector<int>& subsetMaterialArray,		//index value of material for each subset
		std::vector<SurfaceMaterial>& material,		//vector of material structures
		int& subsetCount,				//Number of subsets in mesh
		bool isRHCoordSys,				//true if model was created in right hand coord system
		bool computeNormals);				//true to compute the normals, false to use the files normals

private:
	// Our basic shaders for this example
	/*ID3D11PixelShader* pixelShader;
	ID3D11VertexShader* vertexShader;*/

	// A few more odds and ends we'll need
	ID3D11Buffer* vsConstantBuffer;
	VertexShaderConstantBuffer vsConstantBufferData;

	ID3D11Buffer* textConstantBuffer;
	VertexShaderConstantBuffer* textConstantBufferData;

	ID3D11Buffer* sphereConstantBuffer;
	VertexShaderConstantBuffer sphereConstantBufferData;

	// Keeps track of the old mouse position.  Useful for 
	// determining how far the mouse moved in a single frame.
	POINT prevMousePos;

	// My Extra Stuff
	GameEntity ge;
	GameEntity ge2;
	GameEntity ge3;
	Material* ma;
	Material* maSphere;
	//Camera* camera;
	float time;

	Mesh* mish;
	Text* text;

	GameManager manager;

	// Globals
	ID3D11BlendState* Transparency;

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
	

	// http://www.braynzarsoft.net/index.php?p=D3D11WVP
};