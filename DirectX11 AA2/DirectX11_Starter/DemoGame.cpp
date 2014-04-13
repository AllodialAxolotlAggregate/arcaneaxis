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
#include <vector> 
#include <fstream> 
#include <istream> 
#include <sstream>
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

	meshSubsets = 0;

	// Set up buffers and such
	CreateGeometryBuffers();
	LoadShadersAndInputLayout();
	
	Mesh* tempMesh = new Mesh(); 

	// New Camera stuff - camera defined in DXGame
	camera = Camera::GetInstance();
	camera->ComputeMatrices();

	// init GameManager
	GameManager manager();

	time = .01;

	return true;
}

#pragma region ObjLoader

void DemoGame::LoadObjModel(ID3D11Device* device,
	ID3D11DeviceContext* deviceContext,
	std::wstring filename, 
	ID3D11Buffer** vertBuff, 
	ID3D11Buffer** indexBuff,
	std::vector<int>& subsetIndexStart,
	std::vector<int>& subsetMaterialArray,
	std::vector<SurfaceMaterial>& material, 
	int& subsetCount,
	bool isRHCoordSys,
	bool computeNormals)
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

				//New group (Subset)
			case 'g':	//g - defines a group
				checkChar = fileIn.get();
				if(checkChar == ' ')
				{
					subsetIndexStart.push_back(vIndex);		//Start index for this subset
					subsetCount++;
				}
				break;

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

							//Check to make sure there is at least one subset
							if(subsetCount == 0)
							{
								subsetIndexStart.push_back(vIndex);		//Start index for this subset
								subsetCount++;
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

	subsetIndexStart.push_back(vIndex); //There won't be another index start after our last subset, so set it here

	//sometimes "g" is defined at the very top of the file, then again before the first group of faces.
	//This makes sure the first subset does not conatain "0" indices.
	//if(subsetIndexStart[1] == 0)
	//if(subsetIndexStart.size() == 2)
	//{
		if(subsetIndexStart[1] == 0)
		{
			subsetIndexStart.erase(subsetIndexStart.begin()+1);
			meshSubsets--;
		}
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

	std::wstring lastStringRead;
	int matCount = material.size();	//total materials

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

					fileIn >> material[matCount-1].difColor.x;
					fileIn >> material[matCount-1].difColor.y;
					fileIn >> material[matCount-1].difColor.z;

					kdset = true;
				}

				//Ambient Color (We'll store it in diffuse if there isn't a diffuse already)
				if(checkChar == 'a')	
				{					
					checkChar = fileIn.get();	//remove space
					if(!kdset)
					{
						fileIn >> material[matCount-1].difColor.x;
						fileIn >> material[matCount-1].difColor.y;
						fileIn >> material[matCount-1].difColor.z;
					}
				}
				break;

				//Check for transparency
			case 'T':
				checkChar = fileIn.get();
				if(checkChar == 'r')
				{
					checkChar = fileIn.get();	//remove space
					float Transparency;
					fileIn >> Transparency;

					material[matCount-1].difColor.w = Transparency;

					if(Transparency > 0.0f)
						material[matCount-1].transparent = true;
				}
				break;

				//Some obj files specify d for transparency
			case 'd':
				checkChar = fileIn.get();
				if(checkChar == ' ')
				{
					float Transparency;
					fileIn >> Transparency;

					//'d' - 0 being most transparent, and 1 being opaque, opposite of Tr
					Transparency = 1.0f - Transparency;

					material[matCount-1].difColor.w = Transparency;

					if(Transparency > 0.0f)
						material[matCount-1].transparent = true;					
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
											material[matCount-1].texArrayIndex = i;
											material[matCount-1].hasTexture = true;
										}
									}

									//if the texture is not already loaded, load it now
									if(!alreadyLoaded)
									{
										ID3D11ShaderResourceView* tempMeshSRV;
										hr = CreateWICTextureFromFile( device, deviceContext, fileNamePath.c_str(),
                                            NULL, &tempMeshSRV, NULL );
										if(SUCCEEDED(hr))
										{
											textureNameArray.push_back(fileNamePath.c_str());
											material[matCount-1].texArrayIndex = meshSRV.size();
											meshSRV.push_back(tempMeshSRV);
											material[matCount-1].hasTexture = true;
										}
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
								material[matCount-1].transparent = true;
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
										SurfaceMaterial tempMat;
										material.push_back(tempMat);
										fileIn >> material[matCount].matName;
										material[matCount].transparent = false;
										material[matCount].hasTexture = false;
										material[matCount].texArrayIndex = 0;
										matCount++;
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

	//Set the subsets material to the index value
	//of the its material in our material array
	/*for(int i = 0; i < meshSubsets; ++i)
	{
		bool hasMat = false;
		for(int j = 0; j < material.size(); ++j)
		{
			//if(meshMaterials[i] == material[j].matName)
			if(material[i].matName == material[j].matName)
			{
				subsetMaterialArray.push_back(j);
				hasMat = true;
			}
		}
		if(!hasMat)
			subsetMaterialArray.push_back(0); //Use first material in array
	}*/

	std::vector<Vertex> vertices;
	Vertex tempVert;

	//Create our vertices using the information we got 
	//from the file and store them in a vector
	for(int j = 0 ; j < totalVerts; ++j)
	{
		tempVert.Position = vertPos[vertPosIndex[j]];
		tempVert.Normal = vertNorm[vertNormIndex[j]];
		tempVert.UV = vertTexCoord[vertTCIndex[j]];

		vertices.push_back(tempVert);
	}

	//////////////////////Compute Normals///////////////////////////
	//If computeNormals was set to true then we will create our own
	//normals, if it was set to false we will use the obj files normals
	if(computeNormals)
	{
		std::vector<XMFLOAT3> tempNormal;

		//normalized and unnormalized normals
		XMFLOAT3 unnormalized = XMFLOAT3(0.0f, 0.0f, 0.0f);

		//Used to get vectors (sides) from the position of the verts
		float vecX, vecY, vecZ;

		//Two edges of our triangle
		XMVECTOR edge1 = XMVectorSet(0.0f, 0.0f, 0.0f, 0.0f);
		XMVECTOR edge2 = XMVectorSet(0.0f, 0.0f, 0.0f, 0.0f);

		//Compute face normals
		for(int i = 0; i < meshTriangles; ++i)
		{
			//Get the vector describing one edge of our triangle (edge 0,2)
			vecX = vertices[indices[(i*3)]].Position.x - vertices[indices[(i*3)+2]].Position.x;
			vecY = vertices[indices[(i*3)]].Position.y - vertices[indices[(i*3)+2]].Position.y;
			vecZ = vertices[indices[(i*3)]].Position.z - vertices[indices[(i*3)+2]].Position.z;		
			edge1 = XMVectorSet(vecX, vecY, vecZ, 0.0f);	//Create our first edge

			//Get the vector describing another edge of our triangle (edge 2,1)
			vecX = vertices[indices[(i*3)+2]].Position.x - vertices[indices[(i*3)+1]].Position.x;
			vecY = vertices[indices[(i*3)+2]].Position.y - vertices[indices[(i*3)+1]].Position.y;
			vecZ = vertices[indices[(i*3)+2]].Position.z - vertices[indices[(i*3)+1]].Position.z;		
			edge2 = XMVectorSet(vecX, vecY, vecZ, 0.0f);	//Create our second edge

			//Cross multiply the two edge vectors to get the un-normalized face normal
			XMStoreFloat3(&unnormalized, XMVector3Cross(edge1, edge2));
			tempNormal.push_back(unnormalized);			//Save unormalized normal (for normal averaging)
		}

		//Compute vertex normals (normal Averaging)
		XMVECTOR normalSum = XMVectorSet(0.0f, 0.0f, 0.0f, 0.0f);
		int facesUsing = 0;
		float tX;
		float tY;
		float tZ;

		//Go through each vertex
		for(int i = 0; i < totalVerts; ++i)
		{
			//Check which triangles use this vertex
			for(int j = 0; j < meshTriangles; ++j)
			{
				if(indices[j*3] == i ||
					indices[(j*3)+1] == i ||
					indices[(j*3)+2] == i)
				{
					tX = XMVectorGetX(normalSum) + tempNormal[j].x;
					tY = XMVectorGetY(normalSum) + tempNormal[j].y;
					tZ = XMVectorGetZ(normalSum) + tempNormal[j].z;

					normalSum = XMVectorSet(tX, tY, tZ, 0.0f);	//If a face is using the vertex, add the unormalized face normal to the normalSum
					facesUsing++;
				}
			}

			//Get the actual normal by dividing the normalSum by the number of faces sharing the vertex
			normalSum = normalSum / facesUsing;

			//Normalize the normalSum vector
			normalSum = XMVector3Normalize(normalSum);

			//Store the normal in our current vertex
			vertices[i].Normal.x = XMVectorGetX(normalSum);
			vertices[i].Normal.y = XMVectorGetY(normalSum);
			vertices[i].Normal.z = XMVectorGetZ(normalSum);

			//Clear normalSum and facesUsing for next vertex
			normalSum = XMVectorSet(0.0f, 0.0f, 0.0f, 0.0f);
			facesUsing = 0;

		}
	}

	//Create index buffer
	D3D11_BUFFER_DESC indexBufferDesc;
	ZeroMemory( &indexBufferDesc, sizeof(indexBufferDesc) );

	indexBufferDesc.Usage = D3D11_USAGE_DEFAULT;
	indexBufferDesc.ByteWidth = sizeof(DWORD) * meshTriangles*3;
	indexBufferDesc.BindFlags = D3D11_BIND_INDEX_BUFFER;
	indexBufferDesc.CPUAccessFlags = 0;
	indexBufferDesc.MiscFlags = 0;

	D3D11_SUBRESOURCE_DATA iinitData;

	iinitData.pSysMem = &indices[0];
	device->CreateBuffer(&indexBufferDesc, &iinitData, indexBuff);

	//Create Vertex Buffer
	D3D11_BUFFER_DESC vertexBufferDesc;
	ZeroMemory( &vertexBufferDesc, sizeof(vertexBufferDesc) );

	vertexBufferDesc.Usage = D3D11_USAGE_DEFAULT;
	vertexBufferDesc.ByteWidth = sizeof( Vertex ) * totalVerts;
	vertexBufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	vertexBufferDesc.CPUAccessFlags = 0;
	vertexBufferDesc.MiscFlags = 0;

	D3D11_SUBRESOURCE_DATA vertexBufferData; 

	ZeroMemory( &vertexBufferData, sizeof(vertexBufferData) );
	vertexBufferData.pSysMem = &vertices[0];
	hr = device->CreateBuffer( &vertexBufferDesc, &vertexBufferData, vertBuff);
}

#pragma endregion

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
	maSphere = new Material(); // obj material

	mish = new Mesh[3];
	mish[0] = Mesh(vertices, indices, ARRAYSIZE(vertices), ARRAYSIZE(indices), device);
	mish[1] = Mesh(vertices, indices2, 6, 15, device);
	//mish[2] = Mesh(vertices, indices3, 6, 3, device);
	mish[2] = Mesh(); // obj mesh
	mish[3] = Mesh(); // obj mesh

	ge = GameEntity(&mish[0], ma, XMFLOAT3(1.0, 1.0, 1.0));
	ge2 = GameEntity(&mish[1], ma, XMFLOAT3(0.0, 0.0, 0.0));
	//ge3 = GameEntity(&mish[2], ma, XMFLOAT3(-1.0, -1.0, -1.0));
	
	LoadObjModel(device, deviceContext, L"PentaSphere1.obj", &meshVertBuff, &meshIndexBuff, meshSubsetIndexStart, meshSubsetTexture, material, meshSubsets, true, false);
	LoadObjModel(device, deviceContext, L"sphere.obj", &meshVertBuff1, &meshIndexBuff1, meshSubsetIndexStart, meshSubsetTexture, material, meshSubsets, true, false);
	

	// setup obj Mesh
	mish[2].SetVertexBuffer(meshVertBuff);
	mish[2].SetIndexBuffer(meshIndexBuff);
	mish[3].SetVertexBuffer(meshVertBuff1);
	mish[3].SetIndexBuffer(meshIndexBuff1);

	// setup obj Material
	D3D11_SAMPLER_DESC samplerDesc = maSphere->SamplerDescription();
	ID3D11SamplerState* samplerTemp = maSphere->GetSamplerState();
	device->CreateSamplerState(&samplerDesc, &samplerTemp);
	maSphere->SetSamplerState(samplerTemp);

	// setup obj gameEntity
	obj = GameEntity(&mish[2], maSphere, XMFLOAT3(-5.0, 0.0, 10.0));
	obj1 = GameEntity(&mish[3], maSphere, XMFLOAT3(5.0, 0.0, 10.0));

	entities.push_back(obj);
	entities.push_back(obj1);

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
	maSphere->LoadShadersAndInputLayout(device, vsBlob, psBlob, vertexDesc, ARRAYSIZE(vertexDesc));

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

	// Text Buffer
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

	// Sphere buffer
	D3D11_BUFFER_DESC cBufferDesc2;
	cBufferDesc2.ByteWidth			= sizeof(sphereConstantBufferData);
	cBufferDesc2.Usage				= D3D11_USAGE_DEFAULT;
	cBufferDesc2.BindFlags			= D3D11_BIND_CONSTANT_BUFFER;
	cBufferDesc2.CPUAccessFlags		= 0;
	cBufferDesc2.MiscFlags			= 0;
	cBufferDesc2.StructureByteStride = 0;
	HR(device->CreateBuffer(
		&cBufferDesc2,
		NULL,
		&sphereConstantBuffer));

	maSphere->LoadAConstantBuffer(sphereConstantBuffer);
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
		camera->r_Position.y -= .001;

	if(GetAsyncKeyState('A'))
		camera->r_Position.x += .001;

	if(GetAsyncKeyState('S'))
		camera->r_Position.y += .001;

	if(GetAsyncKeyState('D'))
		camera->r_Position.x -= .001;

	if(GetAsyncKeyState('Q'))
		camera->r_Position.z -= .001;

	if(GetAsyncKeyState('E'))
		camera->r_Position.z += .001;

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
			//ge.Move();
			//ge2.Move();
			//ge3.Move();
			entities[0].Rotate(XMFLOAT3(0,.001,0));
			entities[1].Rotate(XMFLOAT3(.001,0,0));
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

void DemoGame::DrawObj()
{
	UINT stride = sizeof(Vertex);
	UINT offset = 0;

	for(int i = 0; i < entities.size(); i++)  //meshSubsets; ++i)
	{
		/*
		//Set the grounds index buffer
		deviceContext->IASetIndexBuffer( g.GetMesh()->GetIndexBuffer(), DXGI_FORMAT_R32_UINT, 0);
		//Set the grounds vertex buffer
		deviceContext->IASetVertexBuffers( 0, 1, &tempVert, &stride, &offset )*/

		GameEntity g = entities[i];

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

		//g.Draw(deviceContext);

		// Drawing //
		// mat
		ID3D11SamplerState* tempSampler = g.GetMaterial()->GetSamplerState();
		//deviceContext->PSSetShaderResources( 0, 1, &meshSRV[material[meshSubsetTexture[i]].texArrayIndex] );
		deviceContext->PSSetShaderResources( 0, 1, &meshSRV[i] );
		deviceContext->PSSetSamplers(0, 1, &tempSampler);

		// mesh
		ID3D11Buffer* tempVert = g.GetMesh()->GetVertexBuffer();
		deviceContext->IASetVertexBuffers(0, 1, &tempVert, &stride, &offset);
		deviceContext->IASetIndexBuffer(g.GetMesh()->GetIndexBuffer(), DXGI_FORMAT_R32_UINT, 0);

		int indexStart = meshSubsetIndexStart[i];
		int indexDrawAmount =  meshSubsetIndexStart[i+1] - meshSubsetIndexStart[i];
		//int indexDrawAmount =  meshSubsetIndexStart[i];
		//if(!material[meshSubsetTexture[i]].transparent)
			deviceContext->DrawIndexed( indexDrawAmount, indexStart, 0 );
			//deviceContext->DrawIndexed( indexDrawAmount, 0, 0 );

		ID3D11Buffer* tempConst = g.material->r_ConstantBuffer;

		// Set the current vertex and pixel shaders, as well the constant buffer for the vert shader
		deviceContext->VSSetShader(g.material->r_VertexShader, NULL, 0);
		deviceContext->VSSetConstantBuffers(
			0,	// Corresponds to the constant buffer's register in the vertex shader
			1, 
			&tempConst);
		deviceContext->PSSetShader(g.material->r_PixelShader, NULL, 0);
	}
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

	//DrawEntity(ge);
	//DrawEntity(ge2);
	//DrawEntity(ge3);
	DrawObj();

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