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
#define CAMERA_SPEED .01
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
	XMFLOAT4 red	= XMFLOAT4(1.0f, 0.0f, 0.0f, 1.0f);
	XMFLOAT4 green	= XMFLOAT4(0.0f, 1.0f, 0.0f, 1.0f);
	XMFLOAT4 blue	= XMFLOAT4(0.0f, 0.0f, 1.0f, 1.0f);
	XMFLOAT4 white	= XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f);
	XMFLOAT4 orange = XMFLOAT4(1.0f, 1.0f, 0.0f, 1.0f);
	XMFLOAT4 purple = XMFLOAT4(1.0f, 1.0f, 0.0f, 1.0f);

	// Set up the vertices
	Vertex vertices[] = 
	{
		{ XMFLOAT3(0.0f, 0.0f, 0.0f), white, XMFLOAT2(0.5, 0.5), XMFLOAT3(0.0f, 0.0f, 0.0f) },		// 0
		{ XMFLOAT3(+0.0f, +1.0f, +0.0f), red, XMFLOAT2(0.5, 0.0), XMFLOAT3(0.0f, 0.0f, 0.0f) },		// 1
		{ XMFLOAT3(-1.5f, -1.0f, +0.0f), green, XMFLOAT2(0.0, 1.0), XMFLOAT3(0.0f, 0.0f, 0.0f) },	// 2
		{ XMFLOAT3(+1.5f, -1.0f, +0.0f), blue, XMFLOAT2(1.0, 1.0), XMFLOAT3(0.0f, 0.0f, 0.0f) },	// 3
		{ XMFLOAT3(+1.5f, +1.0f, +0.0f), orange, XMFLOAT2(1.0, 0.0), XMFLOAT3(0.0f, 0.0f, 0.0f) },	// 4
		{ XMFLOAT3(-1.5f, +1.5f, +0.0f), purple, XMFLOAT2(0.0, 0.0), XMFLOAT3(0.0f, 0.0f, 0.0f) },	// 5
	};

	UINT indices[] = { 0, 3, 2 };
	UINT indices2[] = { 0, 2, 5,
						0, 5, 1,
						0, 1, 4,
						0, 4, 3,
						0, 3, 2,};
	UINT indices3[] = { 3, 2, 0,
						2, 1, 0};

	ma = new Material[MAX_MATERIAL];
	
	ma[0] = Material(device, deviceContext);
	ma[0].LoadSamplerStateAndShaderResourceView(L"Ignite.jpg");
	ma[1] = Material(device, deviceContext);
	ma[1].LoadSamplerStateAndShaderResourceView(L"wallpaper.jpg");
	ma[2] = Material(device, deviceContext);
	ma[2].LoadSamplerStateAndShaderResourceView(L"rune.png");
	mish = new Mesh[MAX_MESH];
	mish[0] = Mesh(device, deviceContext);
	mish[0].LoadNumbers(ARRAYSIZE(vertices), ARRAYSIZE(indices));
	mish[0].LoadBuffers(vertices, indices);
	mish[1] = Mesh(device, deviceContext);
	mish[1].LoadNumbers(ARRAYSIZE(vertices), ARRAYSIZE(indices2));
	mish[1].LoadBuffers(vertices, indices2);
	mish[2] = Mesh(device, deviceContext);
	mish[2].LoadNumbers(ARRAYSIZE(vertices), ARRAYSIZE(indices3));
	mish[2].LoadBuffers(vertices, indices3);

	ges = new GameEntity[MAX_GAMEENTITY];
	ges[0] = GameEntity(&mish[0], &ma[0], XMFLOAT3(1.0, 1.0, -2.0));
	ges[1] = GameEntity(&mish[1], &ma[1], XMFLOAT3(0.0, 0.0, 0.0));
	ges[2] = GameEntity(&mish[2], &ma[0], XMFLOAT3(-1.0, -1.0, 1.0));

	font = new Font();
	//font->Initialize(device, deviceContext, "fontdata.txt", L"font.jpg");
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

	// masphere is the basis for our Artifact
	maSphere = new Material(device, deviceContext);
	meSphere = new Mesh(device, deviceContext);

	LoadObjModel(L"PentaSphere1.obj", *maSphere, *meSphere, true);

	// Create our Artifact's game entity
	okamaGameSphere = new GameEntity(meSphere, maSphere, XMFLOAT3(-6.0, 0.0, 0.0));

	// ARTIFACT
<<<<<<< HEAD
	//gameArtifact = new Artifact(okamaGameSphere);
	//gameArtifact->GenTiles(objVertices, objListOfIndices, objMeshTraingles);

	/*sentence = new Sentence(device, deviceContext);
	sentence->LoadFontAndShader(font, fShader);
	sentence->Initialize("Gabby, Bob, Andre, and Ryan", 10, 1);*/

	// load and set objs
	//LoadObjModel(L"PentaSphere1.obj", &meshVertBuff, &meshIndexBuff, meshSubsetIndexStart, meshSubsetTexture, material, mish[3], meshSubsets, true, false);
	//LoadObjModel(L"sphere.obj", &meshVertBuff1, &meshIndexBuff1, meshSubsetIndexStart, meshSubsetTexture, material, mish[4], meshSubsets, true, false);

	//// setup obj Mesh
	//mish[3].SetVertexBuffer(meshVertBuff);
	//mish[3].SetIndexBuffer(meshIndexBuff);
	//mish[4].SetVertexBuffer(meshVertBuff1);
	//mish[4].SetIndexBuffer(meshIndexBuff1);

	//// setup obj Material
	//D3D11_SAMPLER_DESC samplerDesc = maSphere->SamplerDescription();
	//ID3D11SamplerState* samplerTemp = maSphere->GetSamplerState();
	//device->CreateSamplerState(&samplerDesc, &samplerTemp);
	//maSphere->SetSamplerState(samplerTemp);

	//// setup obj gameEntity
	//obj = GameEntity(&mish[2], maSphere, XMFLOAT3(-5.0, 0.0, 10.0));
	//obj1 = GameEntity(&mish[3], maSphere, XMFLOAT3(5.0, 0.0, 10.0));

	//entities.push_back(obj);
	//entities.push_back(obj1);
=======
	gameArtifact = new Artifact(okamaGameSphere);

	// 5-8: gmb9280: Commented out because it causes an error in LoadShaders... ()
	//gameArtifact->GenTiles(objVertices, objListOfIndices, objMeshTraingles);

>>>>>>> fb7fbaf7d48270c0ed863dc1d25ca4b3e936a8d5
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

	for(int i = 0; i < MAX_MATERIAL; ++i)
	{
		ma[i].LoadShadersAndInputLayout(L"TextureVertexShader.cso", L"TexturePixelShader.cso", vertexDesc, ARRAYSIZE(vertexDesc));
		ma[i].LoadAConstantBuffer(vsConstantBuffer, &vsConstantBufferData);
	}
	fShader->LoadAConstantBuffer(vsConstantBuffer);

	// Bob's Stuff
	maSphere->LoadShadersAndInputLayout(L"TextureVertexShader.cso", L"TexturePixelShader.cso", vertexDesc, ARRAYSIZE(vertexDesc));
	maSphere->LoadAConstantBuffer(vsConstantBuffer, &vsConstantBufferData);

	// http://www.braynzarsoft.net/index.php?p=D3D11BLENDING#still

	D3D11_BLEND_DESC blendDesc;
	ZeroMemory( &blendDesc, sizeof(blendDesc) );

	D3D11_RENDER_TARGET_BLEND_DESC rtbd;
	ZeroMemory( &rtbd, sizeof(rtbd) );

	// Bryzander Soft
	/*rtbd.BlendEnable			 = true;
	rtbd.SrcBlend				 = D3D11_BLEND_SRC_COLOR;
	rtbd.DestBlend				 = D3D11_BLEND_BLEND_FACTOR;
	rtbd.BlendOp				 = D3D11_BLEND_OP_ADD;
	rtbd.SrcBlendAlpha			 = D3D11_BLEND_ONE;
	rtbd.DestBlendAlpha			 = D3D11_BLEND_ZERO;
	rtbd.BlendOpAlpha			 = D3D11_BLEND_OP_ADD;
	rtbd.RenderTargetWriteMask	 = D3D10_COLOR_WRITE_ENABLE_ALL;*/

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

	if(mish != nullptr)
		delete[] mish;

	if(ma != nullptr)
		delete[] ma;

	if(ges != nullptr)
		delete[] ges;

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

	if(maSphere != nullptr)
		delete maSphere;

	if(meSphere != nullptr)
		delete meSphere;

	/*if(okamaGameSphere != nullptr)
		delete okamaGameSphere;*/

	//gmb9280 : added delete for artifact
	if(gameArtifact != nullptr)
		delete gameArtifact;

	if(objVertices != nullptr)
		delete[] objVertices;

	if(objListOfIndices != nullptr)
		delete[] objListOfIndices;
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
		camera->r_Position.z += CAMERA_SPEED;
		camera->r_Target.z += CAMERA_SPEED;
	}

	if(GetAsyncKeyState('A'))
	{
		camera->r_Position.x -= CAMERA_SPEED;
		camera->r_Target.x -= CAMERA_SPEED;
		//sentence->r_Position.x -= CAMERA_SPEED;
	}

	if(GetAsyncKeyState('Q'))
	{
		camera->r_Position.y -= CAMERA_SPEED;
		camera->r_Target.y -= CAMERA_SPEED;
	}

	if(GetAsyncKeyState('S'))
	{
		camera->r_Position.z -= CAMERA_SPEED;
		camera->r_Target.z -= CAMERA_SPEED;
	}

	if(GetAsyncKeyState('D'))
	{
		camera->r_Position.x += CAMERA_SPEED;
		camera->r_Target.x += CAMERA_SPEED;
		//sentence->r_Position.x += CAMERA_SPEED;
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

	/*if(GetAsyncKeyState(VK_UP))
		sentence->r_Position.y += WORD_SPEED;

	if(GetAsyncKeyState(VK_DOWN))
		sentence->r_Position.y -= WORD_SPEED;

	if(GetAsyncKeyState(VK_RIGHT))
		sentence->r_Position.x += WORD_SPEED;

	if(GetAsyncKeyState(VK_LEFT))
		sentence->r_Position.x -= WORD_SPEED;

	if(GetAsyncKeyState('F'))
		sentence->Initialize("Boop", 10, 1);*/

	//sentence->WorldTransition();
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
		//ge.Move();
		//ge2.Move();
		//ge3.Move();
		
		okamaGameSphere->Rotate(XMFLOAT3(0.001,0,0));
		//okamaGameSphere->MoveTo(XMFLOAT3(prevMousePos.x/5,-prevMousePos.y/5,okamaGameSphere->GetPosition().z));
		
		/*gameArtifact->getGameEntity()->Rotate(XMFLOAT3(0.001,0,0));
		gameArtifact->getGameEntity()->MoveTo(XMFLOAT3(prevMousePos.x/5,-prevMousePos.y/5,okamaGameSphere->GetPosition().z));*/

	
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
	blendFactor[3] = 1.0f;//*/
	/*blendFactor[0] = 1.0f;
	blendFactor[1] = 1.0f;
	blendFactor[2] = 1.0f;
	blendFactor[3] = 1.0f;//*/

	// Turn on the alpha blending.
	deviceContext->OMSetBlendState(Transparency, blendFactor, 0xffffffff);

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

	for(int i = 0; i < MAX_GAMEENTITY; ++i)
		ges[i].Draw();

	okamaGameSphere->Draw();

	// Important to do 2D stuff
	Draw2D();

	//gameArtifact->getGameEntity()->Draw();

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
	float mouseX = (((2.0f * (float)x) / (float) windowWidth) - 1.0f)/(camera->r_ProjectionMatrix._11);
	float mouseY = (((-2.0f * (float)y) / (float) windowHeight) + 1.0f)/(camera->r_ProjectionMatrix._22);

	float newX = (-camera->r_Position.z * mouseX) + camera->r_Position.x;
	float newY = (-camera->r_Position.z * mouseY) + camera->r_Position.y;

	okamaGameSphere->MoveTo(XMFLOAT3(newX, newY, okamaGameSphere->Position.z));

	/*char stringX[10];
	char stringY[10];
	sprintf_s(stringX, 10, "%d", prevMousePos.x);
	sprintf_s(stringY, 10, "%d", prevMousePos.y);
	sentence[0].Initialize(stringX, 1, 10);
	sentence[1].Initialize(stringY, 1, -10);*/
}
#pragma endregion

#pragma region ObjLoader

void DemoGame::LoadObjModel(std::wstring filename,
		Material& material,
		Mesh& mesh,
		bool isRHCoordSys)
{
	HRESULT hr = 0;

	std::wifstream fileIn (filename.c_str());	//Open file
	std::wstring meshMatLib;					//String to hold our obj material library filename

	//Arrays to store our model's information
	std::vector<DWORD> indices;
	std::vector<XMFLOAT3> vertPos;
	std::vector<XMFLOAT3> vertNorm;
	std::vector<XMFLOAT2> vertTexCoord;
	std::vector<std::wstring> meshMaterials;

	//Vertex definition indices
	std::vector<int> vertPosIndex;
	std::vector<int> vertNormIndex;
	std::vector<int> vertTCIndex;

	//Make sure we have a default if no tex coords or normals are defined
	bool hasTexCoord = false;
	bool hasNorm = false;

	//Temp variables to store into vectors
	std::wstring meshMaterialsTemp;
	int vertPosIndexTemp;
	int vertNormIndexTemp;
	int vertTCIndexTemp;

	wchar_t checkChar;		//The variable we will use to store one char from file at a time
	std::wstring face;		//Holds the string containing our face vertices
	int vIndex = 0;			//Keep track of our vertex index count
	int triangleCount = 0;	//Total Triangles
	int totalVerts = 0;
	int meshTriangles = 0;

	// set up meshMaterials to account for previous files
	//meshMaterials.push_back(material.);


#pragma region OBJ File Loader
	//Check to see if the file was opened
	if (fileIn)
	{
		while(fileIn)
		{			
			checkChar = fileIn.get();	//Get next char

			switch (checkChar)
			{		
			case '#':
				checkChar = fileIn.get();
				while(checkChar != '\n')
					checkChar = fileIn.get();
				break;
			case 'v':	//Get Vertex Descriptions
				checkChar = fileIn.get();
				if(checkChar == ' ')	//v - vert position
				{
					float vz, vy, vx;
					fileIn >> vx >> vy >> vz;	//Store the next three types

					if(isRHCoordSys)	//If model is from an RH Coord System
						vertPos.push_back(XMFLOAT3( vx, vy, vz * -1.0f));	//Invert the Z axis
					else
						vertPos.push_back(XMFLOAT3( vx, vy, vz));
				}

				if(checkChar == 't')	//vt - vert tex coords
				{			
					float vtcu, vtcv;
					fileIn >> vtcu >> vtcv;		//Store next two types

					if(isRHCoordSys)	//If model is from an RH Coord System
						vertTexCoord.push_back(XMFLOAT2(vtcu, 1.0f-vtcv));	//Reverse the "v" axis
					else
						vertTexCoord.push_back(XMFLOAT2(vtcu, vtcv));	

					hasTexCoord = true;	//We know the model uses texture coords
				}
				//Since we compute the normals later, we don't need to check for normals
				//In the file, but i'll do it here anyway
				if(checkChar == 'n')	//vn - vert normal
				{
					float vnx, vny, vnz;
					fileIn >> vnx >> vny >> vnz;	//Store next three types

					if(isRHCoordSys)	//If model is from an RH Coord System
						vertNorm.push_back(XMFLOAT3( vnx, vny, vnz * -1.0f ));	//Invert the Z axis
					else
						vertNorm.push_back(XMFLOAT3( vnx, vny, vnz ));	

					hasNorm = true;	//We know the model defines normals
				}
				break;

#pragma region Faces/Indices
			//Get Face Index
			case 'f':	//f - defines the faces
				checkChar = fileIn.get();
				if(checkChar == ' ')
				{
					face = L"";
					std::wstring VertDef;	//Holds one vertex definition at a time
					triangleCount = 0;

					checkChar = fileIn.get();
					while(checkChar != '\n')
					{
						face += checkChar;			//Add the char to our face string
						checkChar = fileIn.get();	//Get the next Character
						if(checkChar == ' ')		//If its a space...
							triangleCount++;		//Increase our triangle count
					}

					//Check for space at the end of our face string
					if(face[face.length()-1] == ' ')
						triangleCount--;	//Each space adds to our triangle count

					triangleCount -= 1;		//Ever vertex in the face AFTER the first two are new faces

					std::wstringstream ss(face);

					if(face.length() > 0)
					{
						int firstVIndex, lastVIndex;	//Holds the first and last vertice's index

						for(int i = 0; i < 3; ++i)		//First three vertices (first triangle)
						{
							ss >> VertDef;	//Get vertex definition (vPos/vTexCoord/vNorm)

							std::wstring vertPart;
							int whichPart = 0;		//(vPos, vTexCoord, or vNorm)

							//Parse this string
							for(int j = 0; j < VertDef.length(); ++j)
							{
								if(VertDef[j] != '/')	//If there is no divider "/", add a char to our vertPart
									vertPart += VertDef[j];

								//If the current char is a divider "/", or its the last character in the string
								if(VertDef[j] == '/' || j ==  VertDef.length()-1)
								{
									std::wistringstream wstringToInt(vertPart);	//Used to convert wstring to int

									if(whichPart == 0)	//If vPos
									{
										wstringToInt >> vertPosIndexTemp;
										vertPosIndexTemp -= 1;		//subtract one since c++ arrays start with 0, and obj start with 1

										//Check to see if the vert pos was the only thing specified
										if(j == VertDef.length()-1)
										{
											vertNormIndexTemp = 0;
											vertTCIndexTemp = 0;
										}
									}

									else if(whichPart == 1)	//If vTexCoord
									{
										if(vertPart != L"")	//Check to see if there even is a tex coord
										{
											wstringToInt >> vertTCIndexTemp;
											vertTCIndexTemp -= 1;	//subtract one since c++ arrays start with 0, and obj start with 1
										}
										else	//If there is no tex coord, make a default
											vertTCIndexTemp = 0;

										//If the cur. char is the second to last in the string, then
										//there must be no normal, so set a default normal
										if(j == VertDef.length()-1)
											vertNormIndexTemp = 0;

									}								
									else if(whichPart == 2)	//If vNorm
									{
										std::wistringstream wstringToInt(vertPart);

										wstringToInt >> vertNormIndexTemp;
										vertNormIndexTemp -= 1;		//subtract one since c++ arrays start with 0, and obj start with 1
									}

									vertPart = L"";	//Get ready for next vertex part
									whichPart++;	//Move on to next vertex part					
								}
							}

							//Avoid duplicate vertices
							bool vertAlreadyExists = false;
							if(totalVerts >= 3)	//Make sure we at least have one triangle to check
							{
								//Loop through all the vertices
								for(int iCheck = 0; iCheck < totalVerts; ++iCheck)
								{
									//If the vertex position and texture coordinate in memory are the same
									//As the vertex position and texture coordinate we just now got out
									//of the obj file, we will set this faces vertex index to the vertex's
									//index value in memory. This makes sure we don't create duplicate vertices
									if(vertPosIndexTemp == vertPosIndex[iCheck] && !vertAlreadyExists)
									{
										if(vertTCIndexTemp == vertTCIndex[iCheck])
										{
											indices.push_back(iCheck);		//Set index for this vertex
											vertAlreadyExists = true;		//If we've made it here, the vertex already exists
										}
									}
								}
							}

							//If this vertex is not already in our vertex arrays, put it there
							if(!vertAlreadyExists)
							{
								vertPosIndex.push_back(vertPosIndexTemp);
								vertTCIndex.push_back(vertTCIndexTemp);
								vertNormIndex.push_back(vertNormIndexTemp);
								totalVerts++;	//We created a new vertex
								indices.push_back(totalVerts-1);	//Set index for this vertex
							}							

							//If this is the very first vertex in the face, we need to
							//make sure the rest of the triangles use this vertex
							if(i == 0)
							{
								firstVIndex = indices[vIndex];	//The first vertex index of this FACE

							}

							//If this was the last vertex in the first triangle, we will make sure
							//the next triangle uses this one (eg. tri1(1,2,3) tri2(1,3,4) tri3(1,4,5))
							if(i == 2)
							{								
								lastVIndex = indices[vIndex];	//The last vertex index of this TRIANGLE
							}
							vIndex++;	//Increment index count
						}

						meshTriangles++;	//One triangle down

						//If there are more than three vertices in the face definition, we need to make sure
						//we convert the face to triangles. We created our first triangle above, now we will
						//create a new triangle for every new vertex in the face, using the very first vertex
						//of the face, and the last vertex from the triangle before the current triangle
						for(int l = 0; l < triangleCount-1; ++l)	//Loop through the next vertices to create new triangles
						{
							//First vertex of this triangle (the very first vertex of the face too)
							indices.push_back(firstVIndex);			//Set index for this vertex
							vIndex++;

							//Second Vertex of this triangle (the last vertex used in the tri before this one)
							indices.push_back(lastVIndex);			//Set index for this vertex
							vIndex++;

							//Get the third vertex for this triangle
							ss >> VertDef;

							std::wstring vertPart;
							int whichPart = 0;

							//Parse this string (same as above)
							for(int j = 0; j < VertDef.length(); ++j)
							{
								if(VertDef[j] != '/')
									vertPart += VertDef[j];
								if(VertDef[j] == '/' || j ==  VertDef.length()-1)
								{
									std::wistringstream wstringToInt(vertPart);

									if(whichPart == 0)
									{
										wstringToInt >> vertPosIndexTemp;
										vertPosIndexTemp -= 1;

										//Check to see if the vert pos was the only thing specified
										if(j == VertDef.length()-1)
										{
											vertTCIndexTemp = 0;
											vertNormIndexTemp = 0;
										}
									}
									else if(whichPart == 1)
									{
										if(vertPart != L"")
										{
											wstringToInt >> vertTCIndexTemp;
											vertTCIndexTemp -= 1;
										}
										else
											vertTCIndexTemp = 0;
										if(j == VertDef.length()-1)
											vertNormIndexTemp = 0;

									}								
									else if(whichPart == 2)
									{
										std::wistringstream wstringToInt(vertPart);

										wstringToInt >> vertNormIndexTemp;
										vertNormIndexTemp -= 1;
									}

									vertPart = L"";
									whichPart++;							
								}
							}					

							//Check for duplicate vertices
							bool vertAlreadyExists = false;
							if(totalVerts >= 3)	//Make sure we at least have one triangle to check
							{
								for(int iCheck = 0; iCheck < totalVerts; ++iCheck)
								{
									if(vertPosIndexTemp == vertPosIndex[iCheck] && !vertAlreadyExists)
									{
										if(vertTCIndexTemp == vertTCIndex[iCheck])
										{
											indices.push_back(iCheck);			//Set index for this vertex
											vertAlreadyExists = true;		//If we've made it here, the vertex already exists
										}
									}
								}
							}

							if(!vertAlreadyExists)
							{
								vertPosIndex.push_back(vertPosIndexTemp);
								vertTCIndex.push_back(vertTCIndexTemp);
								vertNormIndex.push_back(vertNormIndexTemp);
								totalVerts++;					//New vertex created, add to total verts
								indices.push_back(totalVerts-1);		//Set index for this vertex
							}

							//Set the second vertex for the next triangle to the last vertex we got		
							lastVIndex = indices[vIndex];	//The last vertex index of this TRIANGLE

							meshTriangles++;	//New triangle defined
							vIndex++;		
						}
					}
				}
				break;
#pragma endregion 

			case 'm':	//mtllib - material library filename
				checkChar = fileIn.get();
				if(checkChar == 't')
				{
					checkChar = fileIn.get();
					if(checkChar == 'l')
					{
						checkChar = fileIn.get();
						if(checkChar == 'l')
						{
							checkChar = fileIn.get();
							if(checkChar == 'i')
							{
								checkChar = fileIn.get();
								if(checkChar == 'b')
								{
									checkChar = fileIn.get();
									if(checkChar == ' ')
									{
										//Store the material libraries file name
										fileIn >> meshMatLib;
									}
								}
							}
						}
					}
				}

				break;

			case 'u':	//usemtl - which material to use
				checkChar = fileIn.get();
				if(checkChar == 's')
				{
					checkChar = fileIn.get();
					if(checkChar == 'e')
					{
						checkChar = fileIn.get();
						if(checkChar == 'm')
						{
							checkChar = fileIn.get();
							if(checkChar == 't')
							{
								checkChar = fileIn.get();
								if(checkChar == 'l')
								{
									checkChar = fileIn.get();
									if(checkChar == ' ')
									{
										meshMaterialsTemp = L"";	//Make sure this is cleared

										fileIn >> meshMaterialsTemp; //Get next type (string)

										meshMaterials.push_back(meshMaterialsTemp);
									}
								}
							}
						}
					}
				}
				break;

			default:				
				break;
			}
		}
	}
	else	//If we could not open the file
	{
		//SwapChain->SetFullscreenState(false, NULL);	//Make sure we are out of fullscreen

		//create message
		std::wstring message = L"Could not open: ";
		message += filename;

		MessageBox(0, message.c_str(),	//display message
			L"Error", MB_OK);

		return;
	}

	//subsetIndexStart.push_back(vIndex); //There won't be another index start after our last subset, so set it here

	//sometimes "g" is defined at the very top of the file, then again before the first group of faces.
	//This makes sure the first subset does not conatain "0" indices.
	//if(subsetIndexStart[1] == 0)
	//if(subsetIndexStart.size() == 2)
	//{
		/*if(subsetIndexStart[0] == 0)
		{
			subsetIndexStart.erase(subsetIndexStart.begin()+1);
			meshSubsets--;
		}*/
	//}

	//Make sure we have a default for the tex coord and normal
	//if one or both are not specified
	if(!hasNorm)
		vertNorm.push_back(XMFLOAT3(0.0f, 0.0f, 0.0f));
	if(!hasTexCoord)
		vertTexCoord.push_back(XMFLOAT2(0.0f, 0.0f));

	//Close the obj file, and open the mtl file
	fileIn.close();
	fileIn.open(meshMatLib.c_str());
#pragma endregion

#pragma region MTL File Loader

	std::wstring lastStringRead;

	//kdset - If our diffuse color was not set, we can use the ambient color (which is usually the same)
	//If the diffuse color WAS set, then we don't need to set our diffuse color to ambient
	bool kdset = false;

	if (fileIn)
	{
		while(fileIn)
		{
			checkChar = fileIn.get();	//Get next char

			switch (checkChar)
			{
            	//Check for comment
			case '#':
				checkChar = fileIn.get();
				while(checkChar != '\n')
					checkChar = fileIn.get();
				break;

				//Set diffuse color
			case 'K':
				checkChar = fileIn.get();
				if(checkChar == 'd')	//Diffuse Color
				{
					checkChar = fileIn.get();	//remove space

					/*fileIn >> material[matCount-1].difColor.x;
					fileIn >> material[matCount-1].difColor.y;
					fileIn >> material[matCount-1].difColor.z;*/

					kdset = true;
				}

				//Ambient Color (We'll store it in diffuse if there isn't a diffuse already)
				if(checkChar == 'a')	
				{					
					checkChar = fileIn.get();	//remove space
					if(!kdset)
					{
						/*fileIn >> material[matCount-1].difColor.x;
						fileIn >> material[matCount-1].difColor.y;
						fileIn >> material[matCount-1].difColor.z;*/
					}
				}
				break;

				//Check for transparency
			case 'T':
				checkChar = fileIn.get();
				if(checkChar == 'r')
				{
					checkChar = fileIn.get();	//remove space
					/*float Transparency;
					fileIn >> Transparency;

					material[matCount-1].difColor.w = Transparency;

					if(Transparency > 0.0f)
						material[matCount-1].transparent = true*/;
				}
				break;

				//Some obj files specify d for transparency
			case 'd':
				checkChar = fileIn.get();
				if(checkChar == ' ')
				{
					//float Transparency;
					//fileIn >> Transparency;

					////'d' - 0 being most transparent, and 1 being opaque, opposite of Tr
					////Transparency = 1.0f - Transparency;

					////material[matCount-1].difColor.w = Transparency;

					//if(Transparency > 0.0f)
					//	material[matCount-1].transparent = true;					
				}
				break;

				//Get the diffuse map (texture)
			case 'm':
				checkChar = fileIn.get();
				if(checkChar == 'a')
				{
					checkChar = fileIn.get();
					if(checkChar == 'p')
					{
						checkChar = fileIn.get();
						if(checkChar == '_')
						{
							//map_Kd - Diffuse map
							checkChar = fileIn.get();
							if(checkChar == 'K')
							{
								checkChar = fileIn.get();
								if(checkChar == 'd')
								{
									std::wstring fileNamePath;

									fileIn.get();	//Remove whitespace between map_Kd and file

									//Get the file path - We read the pathname char by char since
									//pathnames can sometimes contain spaces, so we will read until
									//we find the file extension
									bool texFilePathEnd = false;
									while(!texFilePathEnd)
									{
										checkChar = fileIn.get();

										fileNamePath += checkChar;

										if(checkChar == '.')
										{
											for(int i = 0; i < 3; ++i)
												fileNamePath += fileIn.get();

											texFilePathEnd = true;
										}							
									}

									//check if this texture has already been loaded
									bool alreadyLoaded = false;
									for(int i = 0; i < textureNameArray.size(); ++i)
									{
										if(fileNamePath == textureNameArray[i])
										{
											alreadyLoaded = true;
											/*material[matCount-1].texArrayIndex = i;
											material[matCount-1].hasTexture = true;*/
										}
									}

									//if the texture is not already loaded, load it now
									if(!alreadyLoaded)
									{
										//ID3D11ShaderResourceView* tempMeshSRV;
										//hr = CreateWICTextureFromFile( device, deviceContext, fileNamePath.c_str(),
                                        //    NULL, &tempMeshSRV, NULL );

										// TO DO: ADD IN UNIQUE MATERIAL


										maSphere->LoadSamplerStateAndShaderResourceView(fileNamePath.c_str());
										textureNameArray.push_back(fileNamePath.c_str());

										/*if(SUCCEEDED(hr))
										{
											textureNameArray.push_back(fileNamePath.c_str());
											material[matCount-1].texArrayIndex = meshSRV.size();
											meshSRV.push_back(tempMeshSRV);
											material[matCount-1].hasTexture = true;
										}*/
									}	
								}
							}
							//map_d - alpha map
							else if(checkChar == 'd')
							{
								//Alpha maps are usually the same as the diffuse map
								//So we will assume that for now by only enabling
								//transparency for this material, as we will already
								//be using the alpha channel in the diffuse map
								//material[matCount-1].transparent = true;
							}
						}
					}
				}
				break;

			case 'n':	//newmtl - Declare new material
				checkChar = fileIn.get();
				if(checkChar == 'e')
				{
					checkChar = fileIn.get();
					if(checkChar == 'w')
					{
						checkChar = fileIn.get();
						if(checkChar == 'm')
						{
							checkChar = fileIn.get();
							if(checkChar == 't')
							{
								checkChar = fileIn.get();
								if(checkChar == 'l')
								{
									checkChar = fileIn.get();
									if(checkChar == ' ')
									{
										//New material, set its defaults
										/*SurfaceMaterial tempMat;
										material.push_back(tempMat);
										fileIn >> material[matCount].matName;
										material[matCount].transparent = false;
										material[matCount].hasTexture = false;
										material[matCount].texArrayIndex = 0;
										matCount++;*/
										kdset = false;
									}
								}
							}
						}
					}
				}
				break;

			default:
				break;
			}
		}
	}	
	else
	{
		//SwapChain->SetFullscreenState(false, NULL);	//Make sure we are out of fullscreen

		std::wstring message = L"Could not open: ";
		message += meshMatLib;

		MessageBox(0, message.c_str(),
			L"Error", MB_OK);

		return;
	}
#pragma endregion

	//Vertex* vertices;
	objVertices = new Vertex[totalVerts];
	Vertex tempVert;

	//Create our vertices using the information we got 
	//from the file and store them in a vector
	for(int j = 0 ; j < totalVerts; ++j)
	{
		tempVert.Position = vertPos[vertPosIndex[j]];
		tempVert.Normal = vertNorm[vertNormIndex[j]];
		tempVert.UV = vertTexCoord[vertTCIndex[j]];

		objVertices[j] = tempVert;
	}	

	//UINT* listOfIndices;
	objListOfIndices = new UINT[indices.size()];
	for(int i = 0; i < indices.size(); ++i)
		objListOfIndices[i] = indices.at(i);

	mesh.LoadNumbers(totalVerts, indices.size());
	mesh.LoadBuffers(objVertices, objListOfIndices);

	// store number of triangles
	objMeshTraingles = meshTriangles;

	/*
	delete[] vertices;
	vertices = nullptr;
	delete[] listOfIndices;
	listOfIndices = nullptr;
	*/
}

#pragma endregion