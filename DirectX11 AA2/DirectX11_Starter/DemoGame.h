#pragma once
#include "GameManager.h"
#include "RyanArtifact.h"

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
	void Draw2D();
	bool PointInFace(Face* _f);

private:
	// Initialization for our "game" demo
	void CreateGeometryBuffers();
	void LoadShadersAndInputLayout();
	void Release();
	bool MouseIsOverEntity(GameEntity* e);
	bool PointInFace(GameEntity* e);
	bool IsInFront(GameEntity* e);

private:
	// A few more odds and ends we'll need
	ID3D11Buffer* vsConstantBuffer;
	VertexShaderConstantBuffer vsConstantBufferData;

	// Keeps track of the old mouse position.  Useful for 
	// determining how far the mouse moved in a single frame.
	POINT prevMousePos;
	float time;

	Font* font;
	FontShader* fShader;
	Sentence* sentence;

	RyanArtifact* ryan;
	// Rotate Globals
	bool artifactTurnLeft;
	bool artifactTurnRight;
	float artifactTurnRateX;
	POINT cursorPos; // gmb9280 testing mouse coords
	POINT windowOffset;
	bool mouseDragging;
	POINT dragStarted;

	float mouseWorldX;
	float mouseWorldY;

	void LockCamera()
	{
		if(camLock != true)
		{
			camLock = true;
		}
	}
	void UnlockCamera()
	{
		if(camLock == true)
		{
			camLock = false;
		}
	}
	bool camLock;
	float elapsedTime;

	GameManager* manager;
	// Menu bools
	bool pausePressed;

	GameEntity* ges;
	Material* ma;
	Mesh* mish;

	// Lighting
	ID3D11Buffer* cbPerFrameBuffer;
	cbPerFrame constbuffPerFrame;
	Light light;
	XMVECTOR lightVector;
};