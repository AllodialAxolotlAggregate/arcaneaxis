#include <Windows.h>
#include <d3dcompiler.h>
#include "DemoGame.h"

#define KEYDOWN(name, key) (name[key] & 0x80)
#define IDENTITY_MATRIX XMFLOAT4X4(1.0, 0.0, 0.0, 0.0, 0.0, 1.0, 0.0, 1.0, 0.0, 0.0, 1.0, 0.0, 0.0, 0.0, 0.0, 1.0);
//#define CAMERA_SPEED .005
#define CAMERA_SPEED .1
#define WORD_SPEED .1

// Maximum number of various things for arrays
#define MAX_MATERIAL 3
#define MAX_MESH 3
#define MAX_GAMEENTITY 3
#define MAX_LINES 2

#pragma region Win32 Entry Point (WinMain)

// Win32 Entry Point
int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE prevInstance,
				   PSTR cmdLine, int showCmd)
{
	// Enable run-time memory check for debug builds.
#if defined(DEBUG) | defined(_DEBUG)
	_CrtSetDbgFlag( _CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF );
#endif

	// Make the game, initialize and run
	DemoGame game(hInstance);
	
	if( !game.Init() )
		return 0;
	
	return game.Run();
}

#pragma endregion

#pragma region Constructor / Destructor

DemoGame::DemoGame(HINSTANCE hInstance) : DXGame(hInstance)
{
	// Set up our custom caption and window size
	windowCaption = L"Arcane Axis";
	windowWidth = 800;
	windowHeight = 600;
}

DemoGame::~DemoGame()
{
	Release();
}

#pragma endregion

#pragma region Initialization

// Initializes the base class (including the window and D3D),
// sets up our geometry and loads the shaders (among other things)
bool DemoGame::Init()
{
	if( !DXGame::Init() )
		return false;

	// Set up buffers and such
	CreateGeometryBuffers();
	LoadShadersAndInputLayout();

	// New Camera stuff - camera defined in DXGame
	camera = Camera::GetInstance();
	camera->ComputeMatrices();

	time = .01;

	return true;
}

// Creates the vertex and index buffers for a single triangle
void DemoGame::CreateGeometryBuffers()
{
	font = new Font();
	font->Initialize(device, deviceContext, "fontdata2.txt", L"font3.dds");
	fShader = new FontShader();
	fShader->Initialize(device, L"FontVertexShader.cso", L"FontPixelShader.cso");

	sentence = new Sentence[MAX_LINES];
	sentence[0] = Sentence(device, deviceContext);
	sentence[0].LoadFontAndShader(font, fShader);
	sentence[0].Initialize("Mouse X", 1, 10);

	sentence[1] = Sentence(device, deviceContext);
	sentence[1].LoadFontAndShader(font, fShader);
	sentence[1].Initialize("Mouse Y", 1, -10);

	ryan = new RyanArtifact(device, deviceContext);
	ryan->LoadOBJs();
}

// Loads shaders from compiled shader object (.cso) files, and uses the
// vertex shader to create an input layout which is needed when sending
// vertex data to the device
void DemoGame::LoadShadersAndInputLayout()
{
	// Set up the vertex layout description
	// This has to match the vertex input layout in the vertex shader
	// We can't set up the input layout yet since we need the actual vert shader
	D3D11_INPUT_ELEMENT_DESC vertexDesc[] =
	{
		{"POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT,	0, 0,	D3D11_INPUT_PER_VERTEX_DATA, 0},
		{"COLOR",    0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, 12,	D3D11_INPUT_PER_VERTEX_DATA, 0},
		{"TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT,		0, 28,	D3D11_INPUT_PER_VERTEX_DATA, 0}
	};


	// Constant buffers ----------------------------------------
	D3D11_BUFFER_DESC cBufferDesc;
	cBufferDesc.ByteWidth			= sizeof(vsConstantBufferData);
	cBufferDesc.Usage				= D3D11_USAGE_DEFAULT;
	cBufferDesc.BindFlags			= D3D11_BIND_CONSTANT_BUFFER;
	cBufferDesc.CPUAccessFlags		= 0;
	cBufferDesc.MiscFlags			= 0;
	cBufferDesc.StructureByteStride = 0;
	HR(device->CreateBuffer(
		&cBufferDesc,
		NULL,
		&vsConstantBuffer)); // When we create the buffer, there is too much stuff for the space allocated I think (only when GenTiles is called).

	ryan->LoadStuff(L"FontVertexShader.cso", L"FontPixelShader.cso", vertexDesc, ARRAYSIZE(vertexDesc), vsConstantBuffer, &vsConstantBufferData, false);
	ryan->LoadStuff(L"TextureVertexShader.cso", L"TexturePixelShader.cso", vertexDesc, ARRAYSIZE(vertexDesc), vsConstantBuffer, &vsConstantBufferData, true);
	ryan->Create(XMFLOAT3(-2.0, 0.0, 0.0));
	fShader->LoadAConstantBuffer(vsConstantBuffer);

	// http://www.braynzarsoft.net/index.php?p=D3D11BLENDING#still
	D3D11_BLEND_DESC blendDesc;
	ZeroMemory( &blendDesc, sizeof(blendDesc) );

	D3D11_RENDER_TARGET_BLEND_DESC rtbd;
	ZeroMemory( &rtbd, sizeof(rtbd) );

	// RasterTek
	rtbd.BlendEnable			 = true;
	rtbd.SrcBlend				 = D3D11_BLEND_ONE;
	rtbd.DestBlend				 = D3D11_BLEND_INV_SRC_ALPHA;
	rtbd.BlendOp				 = D3D11_BLEND_OP_ADD;
	rtbd.SrcBlendAlpha			 = D3D11_BLEND_ONE;
	rtbd.DestBlendAlpha			 = D3D11_BLEND_ZERO;
	rtbd.BlendOpAlpha			 = D3D11_BLEND_OP_ADD;
	rtbd.RenderTargetWriteMask	 = D3D10_COLOR_WRITE_ENABLE_ALL;

	blendDesc.AlphaToCoverageEnable = false;
	blendDesc.RenderTarget[0] = rtbd;

	device->CreateBlendState(&blendDesc, &Transparency);

	D3D11_RASTERIZER_DESC cmdesc;
	ZeroMemory(&cmdesc, sizeof(D3D11_RASTERIZER_DESC));
    
	cmdesc.FillMode = D3D11_FILL_SOLID;
	cmdesc.CullMode = D3D11_CULL_BACK;
    
	cmdesc.FrontCounterClockwise = true;
	device->CreateRasterizerState(&cmdesc, &CCWcullMode);

	cmdesc.FrontCounterClockwise = false;
	device->CreateRasterizerState(&cmdesc, &CWcullMode);
}

void DemoGame::Release()
{
	// Release all of the D3D stuff that's still hanging out
	ReleaseMacro(vsConstantBuffer);
	
	// New Stuff
	delete camera;

	if(font != nullptr)
	{
		font->Shutdown();
		delete font;
	}

	if(fShader != nullptr)
	{
		fShader->Shutdown();
		delete fShader;
	}

	if(sentence != nullptr)
		delete[] sentence;

	if(ryan != nullptr)
		delete ryan;
}

#pragma endregion

#pragma region Window Resizing

// Handles resizing the window and updating our projection matrix to match
void DemoGame::OnResize()
{
	// Handle base-level DX resize stuff
	DXGame::OnResize();

	// Update our projection matrix since the window size changed
	float f = AspectRatio();
	camera->OnResize(f);
}
#pragma endregion

#pragma region Game Loop

void DemoGame::Keyboard()
{
	//http://msdn.microsoft.com/en-us/library/windows/desktop/ms646293(v=vs.85).aspx

	if(GetAsyncKeyState(VK_ESCAPE))
	{
		Release();
		exit(0);
	}

	if(GetAsyncKeyState('W'))
	{
		ryan->MoveTo(XMFLOAT3(-2.0, 0.0, 0.0));
	}

	if(GetAsyncKeyState('A'))
	{
		ryan->Rotate(XMFLOAT3(0.0, -0.01, 0.0));
	}

	if(GetAsyncKeyState('Q'))
	{
		//ryan->Rotate(XMFLOAT3(-0.1, 0.0, 0.0));
	}

	if(GetAsyncKeyState('S'))
	{
		ryan->Rotate(XMFLOAT3(0.01, 0.0, 0.0));
	}

	if(GetAsyncKeyState('D'))
	{
		ryan->Rotate(XMFLOAT3(0.0, 0.01, 0.0));
	}

	if(GetAsyncKeyState('E'))
	{
		camera->r_Position.y += CAMERA_SPEED;
		camera->r_Target.y += CAMERA_SPEED;
	}

	if(GetAsyncKeyState('Z'))
	{
		camera->r_Target.x -= CAMERA_SPEED;
	}

	if(GetAsyncKeyState('X'))
	{
		camera->r_Target.x += CAMERA_SPEED;
	}

	if(GetAsyncKeyState('G'))
	{
		ryan->DontRender();
	}

	camera->ComputeMatrices();
}

// Updates the local constant buffer and 
// push it to the buffer on the device
void DemoGame::UpdateScene(float dt)
{
	time -= dt;
	if(time <= 0)
	{
		time = .001;
	}

	Keyboard();
}

void DemoGame::Draw2D()
{
	//deviceContext->OMSetDepthStencilState
	float blendFactor[4];

	// Setup the blend factor.
	blendFactor[0] = 0.0f;
	blendFactor[1] = 0.0f;
	blendFactor[2] = 0.0f;
	blendFactor[3] = 1.0f;

	// Turn on the alpha blending.
	deviceContext->OMSetBlendState(Transparency, blendFactor, 0xffffffff);

	ryan->Draw();

	for(int i = 0; i < MAX_LINES; ++i)
		sentence[i].Render(camera->r_ViewMatrix, camera->r_ProjectionMatrix);

	// Turn off the alpha blending.
	deviceContext->OMSetBlendState(0, 0, 0xffffffff);
}

// Clear the screen, redraw everything, present
void DemoGame::DrawScene()
{
	const float color[4] = {100/255.0f, 149/255.0f, 237/255.0f, 0.0f};

	// Clear the buffer
	deviceContext->ClearRenderTargetView(renderTargetView, color);
	deviceContext->ClearDepthStencilView(
		depthStencilView, 
		D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL,
		1.0f,
		0);

	vsConstantBufferData.view		= camera->r_ViewMatrix;
	vsConstantBufferData.projection	= camera->r_ProjectionMatrix;
	
	// Important to do 2D stuff
	Draw2D();

	// Present the buffer
	HR(swapChain->Present(0, 0));
}

#pragma endregion

#pragma region Mouse Input

// These methods don't do much currently, but can be used for mouse-related input
// For Triangle collision (faces)
bool DemoGame::PointInFace(GameEntity* e)
{

	XMVECTOR p = XMVectorSet(mouseWorldX, mouseWorldY, 0.0, 0.0);

	XMVECTOR a = XMVectorSet(e->GetMesh()->GetVertices()[0].Position.x, e->GetMesh()->GetVertices()[0].Position.y, 0.0, 0.0);
	XMVECTOR b = XMVectorSet(e->GetMesh()->GetVertices()[1].Position.x, e->GetMesh()->GetVertices()[1].Position.y, 0.0, 0.0);
	XMVECTOR c = XMVectorSet(e->GetMesh()->GetVertices()[2].Position.x, e->GetMesh()->GetVertices()[2].Position.y, 0.0, 0.0);

	a -= p;
	b -= p;
	c -= p;

	XMVECTOR u = XMVector2Cross(b, c);
	XMVECTOR v = XMVector2Cross(c, a);
	XMVECTOR d = XMVector2Dot(u,v);

	if (XMVectorGetByIndex(d,0) < 0.0f)
		return false;

	XMVECTOR w = XMVector2Cross(a,b);
	XMVECTOR ev = XMVector2Dot(u,w);

	if (XMVectorGetByIndex(ev,0) < 0.0f)
		return false;

	return true;
}

void DemoGame::OnMouseDown(WPARAM btnState, int x, int y)
{
	mouseDragging = true;
	this->dragStarted.x = cursorPos.x; 
	this->dragStarted.y = cursorPos.y;

	prevMousePos.x = x;
	prevMousePos.y = y;

	float mouseX = (((2.0f * (float)x) / (float) windowWidth) - 1.0f)/(camera->r_ProjectionMatrix._11);
	float mouseY = (((-2.0f * (float)y) / (float) windowHeight) + 1.0f)/(camera->r_ProjectionMatrix._22);

	float newX = (-camera->r_Position.z * mouseX) + camera->r_Position.x;
	float newY = (-camera->r_Position.z * mouseY) + camera->r_Position.y;

	mouseWorldX = (-camera->r_Position.z * mouseX) + camera->r_Position.x;
	mouseWorldY = (-camera->r_Position.z * mouseY) + camera->r_Position.y;

	XMFLOAT3 rot = XMFLOAT3(0.1, 0.0, 0.0);
	for(int i = 0; i < ryan->NumberOfFace(); i++)
		if(PointInFace(&ryan->GetTile()[i]))
			if(ryan->GetTile()[i].r_Render)
				ryan->GetTile()[i].r_Render = false;

	SetCapture(hMainWnd);
}

void DemoGame::OnMouseUp(WPARAM btnState, int x, int y)
{
	mouseDragging = false;
	ReleaseCapture();
}

void DemoGame::OnMouseMove(WPARAM btnState, int x, int y)
{
	GetCursorPos(&cursorPos);
}

bool DemoGame::MouseIsOverEntity(GameEntity* e)
{
	XMMATRIX x = XMLoadFloat4x4(&e->WorldMatrix);
	XMVECTOR v = XMLoadFloat3(&e->GetMesh()->GetVertices()[2].Position);
	XMVECTOR result = XMVector3Transform(v, x);
	XMFLOAT3 end;
	XMStoreFloat3(&end, result);

	float maxX = end.x;
	float minX = end.x;
	float maxY = end.y;
	float minY = end.y;

	for (int i = 0; i < e->GetMesh()->GetNumberOfVertices(); i++)
	{
		XMFLOAT3 temp = e->mesh->r_Vertices[i].Position;

		XMVECTOR v = XMLoadFloat3(&temp);

		XMVECTOR result2 = XMVector3Transform(v, x);
		XMFLOAT3 current;
		XMStoreFloat3(&current, result2);

		if (current.x > maxX)
			maxX = current.x;

		if (current.y > maxY)
			maxY = current.y;

		if (current.x < minX)
			minX = current.x;

		if (current.y < minY)
			minY = current.y;
	}

	if(mouseWorldX <= maxX && mouseWorldX >= minX
		&& mouseWorldY <= maxY && mouseWorldY >= minY)
		return true;


	return false;
}
#pragma endregion