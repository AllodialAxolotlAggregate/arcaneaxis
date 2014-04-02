// ----------------------------------------------------------------------------
//  A few notes on project settings
//
//  - The project is set to use the UNICODE character set
//    - This was changed in Project Properties > Config Properties > General > Character Set
//    - This basically adds a "#define UNICODE" to the project
//
//  - The include directories were automagically correct, since the DirectX 
//    headers and libs are part of the windows SDK
//    - For instance, $(WindowsSDK_IncludePath) is set as a project include 
//      path by default.  That's where the DirectX headers are located.
//
//  - Two libraries had to be manually added to the Linker Input Dependencies
//    - d3d11.lib
//    - d3dcompiler.lib
//    - This was changed in Project Properties > Config Properties > Linker > Input > Additional Dependencies
//
//  - The Working Directory was changed to match the actual .exe's 
//    output directory, since we need to load the shader files at run time
//    - This was changed in Project Properties > Config Properties > Debugging > Working Directory
//
// ----------------------------------------------------------------------------

#include <Windows.h>
#include <d3dcompiler.h>
#include "DemoGame.h"

#define KEYDOWN(name, key) (name[key] & 0x80)
#define IDENTITY_MATRIX XMFLOAT4X4(1.0, 0.0, 0.0, 0.0, 0.0, 1.0, 0.0, 1.0, 0.0, 0.0, 1.0, 0.0, 0.0, 0.0, 0.0, 1.0);

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
	windowCaption = L"Demo DX11 Game";
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

	// init GameManager
	GameManager manager();

	time = .01;

	return true;
}

// Creates the vertex and index buffers for a single triangle
void DemoGame::CreateGeometryBuffers()
{
	XMFLOAT4 red	= XMFLOAT4(1.0f, 0.0f, 0.0f, 1.0f);
	XMFLOAT4 green	= XMFLOAT4(0.0f, 1.0f, 0.0f, 1.0f);
	XMFLOAT4 blue	= XMFLOAT4(0.0f, 0.0f, 1.0f, 1.0f);
	XMFLOAT4 white	= XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f);
	XMFLOAT4 orange = XMFLOAT4(1.0f, 1.0f, 0.0f, 1.0f);
	XMFLOAT4 purple = XMFLOAT4(1.0f, 1.0f, 0.0f, 1.0f);

	// Set up the vertices
	Vertex vertices[] = 
	{
		{ XMFLOAT3(0.0f, 0.0f, 0.0f), white, XMFLOAT2(0.5, 0.5) },		// 0
		{ XMFLOAT3(+0.0f, +1.0f, +0.0f), red, XMFLOAT2(0.5, 0.0) },		// 1
		{ XMFLOAT3(-1.5f, -1.0f, +0.0f), green, XMFLOAT2(0.0, 1.0) },	// 2
		{ XMFLOAT3(+1.5f, -1.0f, +0.0f), blue, XMFLOAT2(1.0, 1.0) },	// 3
		{ XMFLOAT3(+1.5f, +1.0f, +0.0f), orange, XMFLOAT2(1.0, 0.0) },	// 4
		{ XMFLOAT3(-1.5f, +1.5f, +0.0f), purple, XMFLOAT2(0.0, 0.0) },	// 5
	};

	UINT indices[] = { 0, 3, 2 };
	UINT indices2[] = { 0, 3, 2,
						0, 2, 5,
						0, 5, 1,
						0, 1, 4,
						0, 4, 3 };
	UINT indices3[] = { 3, 2, 0,
						2, 1, 0};

	ma = new Material(device, deviceContext, L"wallpaper.jpg");

	mish = new Mesh[3];
	mish[0] = Mesh(vertices, indices, ARRAYSIZE(vertices), ARRAYSIZE(indices), device);
	mish[1] = Mesh(vertices, indices2, 6, 15, device);
	mish[2] = Mesh(vertices, indices3, 6, 3, device);

	ge = GameEntity(&mish[0], ma, XMFLOAT3(1.0, 1.0, 1.0));
	ge2 = GameEntity(&mish[1], ma, XMFLOAT3(0.0, 0.0, 0.0));
	ge3 = GameEntity(&mish[2], ma, XMFLOAT3(-1.0, -1.0, -1.0));

	text = new Text();
	text->Initialize(device, deviceContext, 800, 600, camera->r_ViewMatrix);
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

	D3D11_INPUT_ELEMENT_DESC polygonLayout[] =
	{
		{"POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT,	0, 0,								D3D11_INPUT_PER_VERTEX_DATA, 0},
		{"TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT,		0, D3D11_APPEND_ALIGNED_ELEMENT,	D3D11_INPUT_PER_VERTEX_DATA, 0}
	};

	// Load Vertex Shader --------------------------------------
	ID3DBlob* vsBlob;
	// D3DReadFileToBlob(L"VertexShader.cso", &vsBlob);
	D3DReadFileToBlob(L"TextureVertexShader.cso", &vsBlob);

	// Load Pixel Shader ---------------------------------------
	ID3DBlob* psBlob;
	// D3DReadFileToBlob(L"PixelShader.cso", &psBlob);
	D3DReadFileToBlob(L"TexturePixelShader.cso", &psBlob);

	ma->LoadShadersAndInputLayout(device, vsBlob, psBlob, vertexDesc, ARRAYSIZE(vertexDesc));

	// Clean up
	ReleaseMacro(vsBlob);
	ReleaseMacro(psBlob);

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
		&vsConstantBuffer));

	ma->LoadAConstantBuffer(vsConstantBuffer);

	D3D11_BUFFER_DESC constantBufferDesc;
	constantBufferDesc.Usage = D3D11_USAGE_DYNAMIC;
	constantBufferDesc.ByteWidth = sizeof(textConstantBufferData);
    constantBufferDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
    constantBufferDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
    constantBufferDesc.MiscFlags = 0;
	constantBufferDesc.StructureByteStride = 0;

	device->CreateBuffer(
		&constantBufferDesc,
		NULL,
		&textConstantBuffer);
}

void DemoGame::Release()
{
	// Release all of the D3D stuff that's still hanging out
	ReleaseMacro(vsConstantBuffer);
	
	// New Stuff
	delete camera;

	if(mish != nullptr)
	{
		delete[] mish;
	}

	if(ma != nullptr)
	{
		ma->Release();
		delete ma;
	}

	if(text != nullptr)
	{
		text->Shutdown();
		delete text;
	}
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
		//Release();
		exit(0);
	}

	if(GetAsyncKeyState('W'))
		camera->r_Position.z += .001;

	if(GetAsyncKeyState('A'))
		camera->r_Position.x -= .001;

	if(GetAsyncKeyState('S'))
		camera->r_Position.z -= .001;

	if(GetAsyncKeyState('D'))
		camera->r_Position.x += .001;

	if(GetAsyncKeyState('P') && manager.gameState == game)
		manager.gameState = pause;
	else if(GetAsyncKeyState('P') && manager.gameState == pause)
		manager.gameState = game;

	camera->ComputeMatrices();
}

// Updates the local constant buffer and 
// push it to the buffer on the device
void DemoGame::UpdateScene(float dt)
{
	// Active Game State
	if (manager.gameState == game)
	{
		time -= dt;
		if(time <= 0)
		{
			time = .001;
			ge.Move();
			//ge2.Move();
			//ge3.Move();
		}
	}
	Keyboard();
}

void DemoGame::DrawEntity(GameEntity& g)
{
	// Set up the input assembler
	deviceContext->IASetInputLayout(g.material->r_InputLayout);
	deviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

	vsConstantBufferData.world		= g.WorldMatrix;
	vsConstantBufferData.view		= camera->r_ViewMatrix;
	vsConstantBufferData.projection	= camera->r_ProjectionMatrix;

	deviceContext->UpdateSubresource(
		g.material->r_ConstantBuffer,
		0,			
		NULL,
		&vsConstantBufferData,
		0,
		0);

	g.Draw(deviceContext);

	ID3D11Buffer* temp = g.material->r_ConstantBuffer;

	// Set the current vertex and pixel shaders, as well the constant buffer for the vert shader
	deviceContext->VSSetShader(g.material->r_VertexShader, NULL, 0);
	deviceContext->VSSetConstantBuffers(
		0,	// Corresponds to the constant buffer's register in the vertex shader
		1, 
		&temp);
	deviceContext->PSSetShader(g.material->r_PixelShader, NULL, 0);
}

void DemoGame::Draw2D(GameEntity& g)
{
	deviceContext->OMSetDepthStencilState(m_depthDisabledStencilState, 1);

	float blendFactor[4];

	// Setup the blend factor.
	blendFactor[0] = 0.0f;
	blendFactor[1] = 0.0f;
	blendFactor[2] = 0.0f;
	blendFactor[3] = 0.0f;
	
	// Turn on the alpha blending.
	deviceContext->OMSetBlendState(m_alphaEnableBlendingState, blendFactor, 0xffffffff);

#pragma region Drawing

	// Set up the input assembler
	deviceContext->IASetInputLayout(text->r_FontShader->r_InputLayout);
	deviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

	XMFLOAT4X4 w, ortho = IDENTITY_MATRIX;
	/*XMMATRIX o = XMMatrixOrthographicLH(600, 800, -1, 1);
	XMStoreFloat4x4(&ortho, XMMatrixTranspose(o));*/

	text->Render(deviceContext, w, camera->r_ViewMatrix, camera->r_ProjectionMatrix);

	vsConstantBufferData.world		= g.WorldMatrix;
	vsConstantBufferData.view		= camera->r_ViewMatrix;
	vsConstantBufferData.projection	= camera->r_ProjectionMatrix;
	deviceContext->UpdateSubresource(
		vsConstantBuffer,
		0,			
		NULL,		
		&vsConstantBufferData,
		0,
		0);
	g.Draw(deviceContext);//*/

	// Set the current vertex and pixel shaders, as well the constant buffer for the vert shader
	deviceContext->VSSetShader(text->r_FontShader->r_VertexShader, NULL, 0);
	deviceContext->VSSetConstantBuffers(
		0,	// Corresponds to the constant buffer's register in the vertex shader
		1, 
		&vsConstantBuffer);
	deviceContext->PSSetShader(text->r_FontShader->r_PixelShader, NULL, 0);

#pragma endregion

	// Setup the blend factor.
	blendFactor[0] = 0.0f;
	blendFactor[1] = 0.0f;
	blendFactor[2] = 0.0f;
	blendFactor[3] = 0.0f;
	
	// Turn off the alpha blending.
	deviceContext->OMSetBlendState(m_alphaDisableBlendingState, blendFactor, 0xffffffff);

	deviceContext->OMSetDepthStencilState(m_depthStencilState, 1);
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

	// Important to do 2D stuff
	//Draw2D();

	DrawEntity(ge);
	DrawEntity(ge2);
	DrawEntity(ge3);

	// Present the buffer
	HR(swapChain->Present(0, 0));
}

#pragma endregion

#pragma region Mouse Input

// These methods don't do much currently, but can be used for mouse-related input

void DemoGame::OnMouseDown(WPARAM btnState, int x, int y)
{
	prevMousePos.x = x;
	prevMousePos.y = y;

	SetCapture(hMainWnd);
}

void DemoGame::OnMouseUp(WPARAM btnState, int x, int y)
{
	ReleaseCapture();
}

void DemoGame::OnMouseMove(WPARAM btnState, int x, int y)
{
	prevMousePos.x = x;
	prevMousePos.y = y;
}

#pragma endregion