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
	void CreateGeometryBuffers();
	void LoadShadersAndInputLayout();
	void DrawEntity(GameEntity&);
	void Release();

private:
	// Our basic shaders for this example
	/*ID3D11PixelShader* pixelShader;
	ID3D11VertexShader* vertexShader;*/

	// A few more odds and ends we'll need
	ID3D11Buffer* vsConstantBuffer;
	VertexShaderConstantBuffer vsConstantBufferData;

	ID3D11Buffer* textConstantBuffer;
	VertexShaderConstantBuffer* textConstantBufferData;

	// Keeps track of the old mouse position.  Useful for 
	// determining how far the mouse moved in a single frame.
	POINT prevMousePos;

	// My Extra Stuff
	GameEntity ge;
	GameEntity ge2;
	GameEntity ge3;
	Material* ma;
	//Camera* camera;
	float time;

	Mesh* mish;
	Text* text;

	GameManager manager;
};