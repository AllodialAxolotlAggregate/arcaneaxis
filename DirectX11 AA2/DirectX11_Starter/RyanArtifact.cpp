#include "RyanArtifact.h"

using namespace DirectX;

RyanArtifact::RyanArtifact() :
	m_StoneMaterial(nullptr),
	m_StoneMesh(nullptr),
	m_Stone(nullptr),
	m_RuneMaterial(nullptr),
	m_Position(XMFLOAT3(0.0, 0.0, 0.0)),
	velocX(0),
	velocY(0),
	accX(0),
	accY(0)
{
}

RyanArtifact::RyanArtifact(ID3D11Device* device, ID3D11DeviceContext* deviceContext) :
	m_Stone(nullptr),
	m_Position(XMFLOAT3(0.0, 0.0, 0.0)),
	velocX(0),
	velocY(0),
	accX(0),
	accY(0)
{
	m_StoneMaterial = new Material(device, deviceContext);
	m_StoneMesh = new Mesh(device, deviceContext);

	m_RuneMaterial = new Material(device, deviceContext);
}

RyanArtifact::~RyanArtifact()
{
	if(m_StoneMaterial != nullptr)
	{
		delete m_StoneMaterial;
		m_StoneMaterial = nullptr;
	}

	if(m_StoneMesh != nullptr)
	{
		delete m_StoneMesh;
		m_StoneMesh = nullptr;
	}

	if(m_Stone != nullptr)
	{
		delete m_Stone;
		m_Stone = nullptr;
	}

	if(m_RuneMaterial != nullptr)
	{
		delete m_RuneMaterial;
		m_RuneMaterial = nullptr;
	}

	if(objVertices != nullptr)
		delete[] objVertices;

	if(objListOfIndices != nullptr)
		delete[] objListOfIndices;

	if(m_ManyMeshes != nullptr)
	{
		delete[] m_ManyMeshes;
		m_ManyMeshes = nullptr;
	}

	if(m_Tiles != nullptr)
	{
		delete[] m_Tiles;
		m_Tiles = nullptr;
	}
}

void RyanArtifact::Draw()
{
	//m_Rune->Draw();
	m_Stone->Draw();

	for(int i = 0; i < m_StoneMesh->r_NumberOfFaces; ++i)
			m_Tiles[i].Draw();
}

void RyanArtifact::LoadStuff(const wchar_t* vsFile, const wchar_t* psFile, D3D11_INPUT_ELEMENT_DESC* vertexDesc, SIZE_T arraySize,ID3D11Buffer* aCSBuffer, VertexShaderConstantBuffer* aConstantBufferData, bool whichOne)
{
	if(whichOne)
	{
		m_StoneMaterial->LoadShadersAndInputLayout(vsFile, psFile, vertexDesc, arraySize);
		m_StoneMaterial->LoadAConstantBuffer(aCSBuffer, aConstantBufferData);
		m_StoneMaterial->LoadSamplerStateAndShaderResourceView(L"gem_diffuse.jpg");
	}else
	{
		m_RuneMaterial->LoadShadersAndInputLayout(vsFile, psFile, vertexDesc, arraySize);
		m_RuneMaterial->LoadAConstantBuffer(aCSBuffer, aConstantBufferData);
		m_RuneMaterial->LoadSamplerStateAndShaderResourceView(L"font3.dds");
	}
}

void RyanArtifact::Rotate(DirectX::XMFLOAT3 rotation)
{
	m_Stone->Rotate(rotation);
	for(int i = 0; i < m_StoneMesh->r_NumberOfFaces; ++i)
			m_Tiles[i].Rotate(rotation);
}

void RyanArtifact::MoveTo(DirectX::XMFLOAT3 position)
{
	m_Stone->MoveTo(position);
	for(int i = 0; i < m_StoneMesh->r_NumberOfFaces; ++i)
			m_Tiles[i].MoveTo(position);
}

void RyanArtifact::DontRender()
{
	if(m_Stone->r_Render)
		m_Stone->r_Render = false;
	else
		m_Stone->r_Render = true;
}

void RyanArtifact::LoadOBJs()
{
	LoadObjModel(L"PentaSphere.obj", *m_StoneMesh, true);

	int numOfFaces = m_StoneMesh->r_NumberOfFaces;
	int numOfIndices = m_StoneMesh->r_NumberOfIndices;

	Font* m_Font = new Font();
	m_Font->Initialize(m_StoneMesh->r_Device, m_StoneMesh->r_DeviceContext, "fontdata2.txt", L"font3.dds");

	char* words = "12345";
	int numLetters = strlen(words);
	FontVertex* vertices2 = new FontVertex[6*numLetters];
	memset(vertices2, 0, (sizeof(FontVertex) * 6*numLetters));
	m_Font->BuildVertexArray((void*)vertices2, words, 0.0, 0.0);

	Vertex* verts = new Vertex[numOfIndices];
	UINT* inds = new UINT[numOfIndices];

	// Test stuff
	m_Tiles = new GameEntity[numOfFaces];
	m_ManyMeshes = new Mesh[numOfIndices];

	for(int i = 0; i < numOfFaces; ++i)
	{
		int r = (rand() % (2*numLetters));
		/*int r = 0;
		if(i%2)
			r = 1;*/
		Vertex vertices[] = 
		{
			Vertex(m_StoneMesh->r_Faces[i].r_Vertices[0].Position, m_StoneMesh->r_Faces[i].r_Vertices[0].Color, vertices2[3*r].UV, m_StoneMesh->r_Faces[i].r_Vertices[0].Normal),
			Vertex(m_StoneMesh->r_Faces[i].r_Vertices[1].Position, m_StoneMesh->r_Faces[i].r_Vertices[1].Color, vertices2[3*r+1].UV, m_StoneMesh->r_Faces[i].r_Vertices[1].Normal),
			Vertex(m_StoneMesh->r_Faces[i].r_Vertices[2].Position, m_StoneMesh->r_Faces[i].r_Vertices[2].Color, vertices2[3*r+2].UV, m_StoneMesh->r_Faces[i].r_Vertices[2].Normal)
		};

		verts[3*i] = vertices[0];
		verts[3*i+1] = vertices[1];
		verts[3*i+2] = vertices[2];

		inds[3*i] = m_StoneMesh->r_Faces[i].r_Indices[0];
		inds[3*i+1] = m_StoneMesh->r_Faces[i].r_Indices[1];
		inds[3*i+2] = m_StoneMesh->r_Faces[i].r_Indices[2];

		m_ManyMeshes[i] = Mesh(m_StoneMesh->r_Device, m_StoneMesh->r_DeviceContext);
		m_ManyMeshes[i].LoadNumbers(3, 3);
		m_ManyMeshes[i].LoadBuffers(vertices, m_StoneMesh->r_Faces->r_Indices);

		m_Tiles[i] = GameEntity(&m_ManyMeshes[i], m_RuneMaterial, XMFLOAT3(0.0, 0.0, 0.0));
	}

	delete m_Font;
	delete[] vertices2;
	delete[] verts;
	delete[] inds;
}

void RyanArtifact::Create(DirectX::XMFLOAT3 origin)
{
	m_Position = origin;

	m_StoneMesh->CreateBuffers();

	m_Stone = new GameEntity(m_StoneMesh, m_StoneMaterial, m_Position);
	for(int i = 0; i < m_StoneMesh->r_NumberOfFaces; ++i)
	{
		m_Tiles[i].ScaleWithFloat(1.05);
		m_Tiles[i].MoveTo(origin);
	}
}

// Spin according to our physics
void RyanArtifact::Spin()
{
	// Add acceleration accrued
	this->velocX += this->accX;
	this->velocY += this->accY;
	this->accY = 0; this->accX = 0;

	this->velocX *= .97;
	this->velocY *= .97;
	
	this->Rotate( XMFLOAT3(velocX, velocY, 0));
}

// When mouse stuff happens, pass it back here
void RyanArtifact::AddAccel(float x, float y)
{
	this->accX += x; 
	this->accY += y;
}

#pragma region ObjLoader

void RyanArtifact::LoadObjModel(std::wstring filename,
		Mesh& mesh,
		bool isRHCoordSys)
{
	HRESULT hr = 0;

	std::wifstream fileIn (filename.c_str());	//Open file
	std::wstring meshMatLib;					//String to hold our obj material library filename

	//Arrays to store our model's information
	std::vector<DWORD> indices;
	std::vector<DirectX::XMFLOAT3> vertPos;
	std::vector<DirectX::XMFLOAT3> vertNorm;
	std::vector<DirectX::XMFLOAT2> vertTexCoord;
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
						vertPos.push_back(DirectX::XMFLOAT3( vx, vy, vz));	//Invert the Z axis
					else
						vertPos.push_back(DirectX::XMFLOAT3( vx, vy, vz));
				}

				if(checkChar == 't')	//vt - vert tex coords
				{			
					float vtcu, vtcv;
					fileIn >> vtcu >> vtcv;		//Store next two types

					if(isRHCoordSys)	//If model is from an RH Coord System
						vertTexCoord.push_back(DirectX::XMFLOAT2(vtcu, 1.0f-vtcv));	//Reverse the "v" axis
					else
						vertTexCoord.push_back(DirectX::XMFLOAT2(vtcu, vtcv));	

					hasTexCoord = true;	//We know the model uses texture coords
				}
				//Since we compute the normals later, we don't need to check for normals
				//In the file, but i'll do it here anyway
				if(checkChar == 'n')	//vn - vert normal
				{
					float vnx, vny, vnz;
					fileIn >> vnx >> vny >> vnz;	//Store next three types

					if(isRHCoordSys)	//If model is from an RH Coord System
						vertNorm.push_back(DirectX::XMFLOAT3( vnx, vny, vnz * -1.0f ));	//Invert the Z axis
					else
						vertNorm.push_back(DirectX::XMFLOAT3( vnx, vny, vnz ));	

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
		vertNorm.push_back(DirectX::XMFLOAT3(0.0f, 0.0f, 0.0f));
	if(!hasTexCoord)
		vertTexCoord.push_back(DirectX::XMFLOAT2(0.0f, 0.0f));

	//Close the obj file, and open the mtl file
	fileIn.close();
	fileIn.open(meshMatLib.c_str());
#pragma u
//
//#pragma region MTL File Loader
//
//	std::wstring lastStringRead;
//
//	//kdset - If our diffuse color was not set, we can use the ambient color (which is usually the same)
//	//If the diffuse color WAS set, then we don't need to set our diffuse color to ambient
//	bool kdset = false;
//
//	if (fileIn)
//	{
//		while(fileIn)
//		{
//			checkChar = fileIn.get();	//Get next char
//
//			switch (checkChar)
//			{
//            	//Check for comment
//			case '#':
//				checkChar = fileIn.get();
//				while(checkChar != '\n')
//					checkChar = fileIn.get();
//				break;
//
//				//Set diffuse color
//			case 'K':
//				checkChar = fileIn.get();
//				if(checkChar == 'd')	//Diffuse Color
//				{
//					checkChar = fileIn.get();	//remove space
//
//					/*fileIn >> material[matCount-1].difColor.x;
//					fileIn >> material[matCount-1].difColor.y;
//					fileIn >> material[matCount-1].difColor.z;*/
//
//					kdset = true;
//				}
//
//				//Ambient Color (We'll store it in diffuse if there isn't a diffuse already)
//				if(checkChar == 'a')	
//				{					
//					checkChar = fileIn.get();	//remove space
//					if(!kdset)
//					{
//						/*fileIn >> material[matCount-1].difColor.x;
//						fileIn >> material[matCount-1].difColor.y;
//						fileIn >> material[matCount-1].difColor.z;*/
//					}
//				}
//				break;
//
//				//Check for transparency
//			case 'T':
//				checkChar = fileIn.get();
//				if(checkChar == 'r')
//				{
//					checkChar = fileIn.get();	//remove space
//					/*float Transparency;
//					fileIn >> Transparency;
//
//					material[matCount-1].difColor.w = Transparency;
//
//					if(Transparency > 0.0f)
//						material[matCount-1].transparent = true*/;
//				}
//				break;
//
//				//Some obj files specify d for transparency
//			case 'd':
//				checkChar = fileIn.get();
//				if(checkChar == ' ')
//				{
//					//float Transparency;
//					//fileIn >> Transparency;
//
//					////'d' - 0 being most transparent, and 1 being opaque, opposite of Tr
//					////Transparency = 1.0f - Transparency;
//
//					////material[matCount-1].difColor.w = Transparency;
//
//					//if(Transparency > 0.0f)
//					//	material[matCount-1].transparent = true;					
//				}
//				break;
//
//				//Get the diffuse map (texture)
//			case 'm':
//				checkChar = fileIn.get();
//				if(checkChar == 'a')
//				{
//					checkChar = fileIn.get();
//					if(checkChar == 'p')
//					{
//						checkChar = fileIn.get();
//						if(checkChar == '_')
//						{
//							//map_Kd - Diffuse map
//							checkChar = fileIn.get();
//							if(checkChar == 'K')
//							{
//								checkChar = fileIn.get();
//								if(checkChar == 'd')
//								{
//									std::wstring fileNamePath;
//
//									fileIn.get();	//Remove whitespace between map_Kd and file
//
//									//Get the file path - We read the pathname char by char since
//									//pathnames can sometimes contain spaces, so we will read until
//									//we find the file extension
//									bool texFilePathEnd = false;
//									while(!texFilePathEnd)
//									{
//										checkChar = fileIn.get();
//
//										fileNamePath += checkChar;
//
//										if(checkChar == '.')
//										{
//											for(int i = 0; i < 3; ++i)
//												fileNamePath += fileIn.get();
//
//											texFilePathEnd = true;
//										}							
//									}
//
//									//check if this texture has already been loaded
//									bool alreadyLoaded = false;
//									for(int i = 0; i < textureNameArray.size(); ++i)
//									{
//										if(fileNamePath == textureNameArray[i])
//										{
//											alreadyLoaded = true;
//											/*material[matCount-1].texArrayIndex = i;
//											material[matCount-1].hasTexture = true;*/
//										}
//									}
//
//									//if the texture is not already loaded, load it now
//									if(!alreadyLoaded)
//									{
//										//ID3D11ShaderResourceView* tempMeshSRV;
//										//hr = CreateWICTextureFromFile( device, deviceContext, fileNamePath.c_str(),
//                                        //    NULL, &tempMeshSRV, NULL );
//
//										// TO DO: ADD IN UNIQUE MATERIAL
//
//
//										//material.LoadSamplerStateAndShaderResourceView(fileNamePath.c_str());
//										textureNameArray.push_back(fileNamePath.c_str());
//
//										/*if(SUCCEEDED(hr))
//										{
//											textureNameArray.push_back(fileNamePath.c_str());
//											material[matCount-1].texArrayIndex = meshSRV.size();
//											meshSRV.push_back(tempMeshSRV);
//											material[matCount-1].hasTexture = true;
//										}*/
//									}	
//								}
//							}
//							//map_d - alpha map
//							else if(checkChar == 'd')
//							{
//								//Alpha maps are usually the same as the diffuse map
//								//So we will assume that for now by only enabling
//								//transparency for this material, as we will already
//								//be using the alpha channel in the diffuse map
//								//material[matCount-1].transparent = true;
//							}
//						}
//					}
//				}
//				break;
//
//			case 'n':	//newmtl - Declare new material
//				checkChar = fileIn.get();
//				if(checkChar == 'e')
//				{
//					checkChar = fileIn.get();
//					if(checkChar == 'w')
//					{
//						checkChar = fileIn.get();
//						if(checkChar == 'm')
//						{
//							checkChar = fileIn.get();
//							if(checkChar == 't')
//							{
//								checkChar = fileIn.get();
//								if(checkChar == 'l')
//								{
//									checkChar = fileIn.get();
//									if(checkChar == ' ')
//									{
//										//New material, set its defaults
//										/*SurfaceMaterial tempMat;
//										material.push_back(tempMat);
//										fileIn >> material[matCount].matName;
//										material[matCount].transparent = false;
//										material[matCount].hasTexture = false;
//										material[matCount].texArrayIndex = 0;
//										matCount++;*/
//										kdset = false;
//									}
//								}
//							}
//						}
//					}
//				}
//				break;
//
//			default:
//				break;
//			}
//		}
//	}	
//	else
//	{
//		//SwapChain->SetFullscreenState(false, NULL);	//Make sure we are out of fullscreen
//
//		std::wstring message = L"Could not open: ";
//		message += meshMatLib;
//
//		MessageBox(0, message.c_str(),
//			L"Error", MB_OK);
//
//		return;
//	}
//#pragma endregion

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
	mesh.LoadVertices(objVertices, objListOfIndices);

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