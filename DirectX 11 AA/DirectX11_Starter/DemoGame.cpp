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
	// Release all of the D3D stuff that's still hanging out
	ReleaseMacro(vertexBuffer);
	ReleaseMacro(indexBuffer);
	ReleaseMacro(vertexShader);
	ReleaseMacro(pixelShader);
	ReleaseMacro(vsConstantBuffer);
	ReleaseMacro(inputLayout);

	if(mish != nullptr)
		delete[] mish;

	if(ma != nullptr)
		delete ma;
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

	// Set up view matrix (camera)
	// In an actual game, update this when the camera moves (so every frame)
	XMVECTOR position	= XMVectorSet(0, 0, -5, 0);
	XMVECTOR target		= XMVectorSet(0, 0, 0, 0);
	XMVECTOR up			= XMVectorSet(0, 1, 0, 0);
	XMMATRIX V			= XMMatrixLookAtLH(position, target, up);
	XMStoreFloat4x4(&viewMatrix, XMMatrixTranspose(V));

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

	// http://my.safaribooksonline.com/book/programming/game-programming/9781435458956/2d-rendering/ch03lev1sec4
	D3D11_SAMPLER_DESC samplerDesc;
	samplerDesc.Filter = D3D11_FILTER_MIN_MAG_MIP_LINEAR;
	samplerDesc.AddressU = D3D11_TEXTURE_ADDRESS_WRAP;
	samplerDesc.AddressV = D3D11_TEXTURE_ADDRESS_WRAP;
	samplerDesc.AddressW = D3D11_TEXTURE_ADDRESS_WRAP;
	samplerDesc.MipLODBias = 0;
	samplerDesc.MaxAnisotropy = 0;
	samplerDesc.MinLOD = 0;
	samplerDesc.MaxLOD = 0;

	ID3D11SamplerState* ss;
	device->CreateSamplerState(&samplerDesc, &ss);
	ma = new Material(device, deviceContext, ss, L"wallpaper.jpg");

	mish = new Mesh[3];
	mish[0] = Mesh(vertices, indices, 6, 3, device);
	mish[1] = Mesh(vertices, indices2, 6, 15, device);
	mish[2] = Mesh(vertices, indices3, 6, 3, device);

	ge = GameEntity(&mish[0], ma, XMFLOAT3(1.0, 1.0, 1.0));
	ge2 = GameEntity(&mish[1], ma, XMFLOAT3(0.0, 0.0, 0.0));
	ge3 = GameEntity(&mish[2], ma, XMFLOAT3(-1.0, -1.0, -1.0));
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

	// Load Vertex Shader --------------------------------------
	ID3DBlob* vsBlob;
	D3DReadFileToBlob(L"TextureVertexShader.cso", &vsBlob);

	// Create the shader on the device
	HR(device->CreateVertexShader(
		vsBlob->GetBufferPointer(),
		vsBlob->GetBufferSize(),
		NULL,
		&vertexShader));

	// Before cleaning up the data, create the input layout
	HR(device->CreateInputLayout(
		vertexDesc,
		ARRAYSIZE(vertexDesc),
		vsBlob->GetBufferPointer(),
		vsBlob->GetBufferSize(),
		&inputLayout));

	// Clean up
	ReleaseMacro(vsBlob);

	// Load Pixel Shader ---------------------------------------
	ID3DBlob* psBlob;
	D3DReadFileToBlob(L"TexturePixelShader.cso", &psBlob);

	// Create the shader on the device
	HR(device->CreatePixelShader(
		psBlob->GetBufferPointer(),
		psBlob->GetBufferSize(),
		NULL,
		&pixelShader));

	// Clean up
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
}

#pragma endregion

#pragma region Window Resizing

// Handles resizing the window and updating our projection matrix to match
void DemoGame::OnResize()
{
	// Handle base-level DX resize stuff
	DXGame::OnResize();

	// Update our projection matrix since the window size changed
	XMMATRIX P = XMMatrixPerspectiveFovLH(
		0.25f * 3.1415926535f,
		AspectRatio(),
		0.1f,
		100.0f);
	XMStoreFloat4x4(&projectionMatrix, XMMatrixTranspose(P));
}
#pragma endregion

#pragma region Game Loop

// Updates the local constant buffer and 
// push it to the buffer on the device
void DemoGame::UpdateScene(float dt)
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

void DemoGame::DrawEntity(GameEntity& g)
{
	vsConstantBufferData.world		= g.WorldMatrix;
	vsConstantBufferData.view		= viewMatrix;
	vsConstantBufferData.projection	= projectionMatrix;
	deviceContext->UpdateSubresource(
		vsConstantBuffer,
		0,			
		NULL,		
		&vsConstantBufferData,
		0,
		0);
	g.Draw(deviceContext);
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

	// Set up the input assembler
	deviceContext->IASetInputLayout(inputLayout);
	deviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

	DrawEntity(ge);
	DrawEntity(ge2);
	DrawEntity(ge3);

	// Set the current vertex and pixel shaders, as well the constant buffer for the vert shader
	deviceContext->VSSetShader(vertexShader, NULL, 0);
	deviceContext->VSSetConstantBuffers(
		0,	// Corresponds to the constant buffer's register in the vertex shader
		1, 
		&vsConstantBuffer);
	deviceContext->PSSetShader(pixelShader, NULL, 0);

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