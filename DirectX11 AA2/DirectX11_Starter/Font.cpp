#include "Font.h"

using namespace DirectX;
#define UV_HEIGHT 0.9f

Font::Font() :
	m_Font(nullptr),
	m_ShaderResourceView(nullptr)
{}
Font::~Font() {}

void Font::Initialize(ID3D11Device* device,ID3D11DeviceContext* deviceContext, char* fontFilename, WCHAR* UVFilename)
{
	// Load in the text file containing the font data
	LoadFontData(fontFilename);
	//CreateWICTextureFromFile(device, deviceContext, UVFilename, 0, &m_ShaderResourceView);
	CreateDDSTextureFromFile(device, UVFilename, nullptr, &m_ShaderResourceView);
	//HRESULT hr = CreateDDSTextureFromFile(device, L"font2.dds", nullptr, &m_ShaderResourceView);
	/*if (FAILED(hr))
		CreateWICTextureFromFile(device, deviceContext, L"Ignite.jpg", 0, &m_ShaderResourceView);//*/
}

void Font::Shutdown()
{
	if(m_Font)
	{
		delete[] m_Font;
		m_Font = 0;
	}

	if(m_ShaderResourceView)
	{
		m_ShaderResourceView->Release();
		m_ShaderResourceView = 0;
	}
}

void Font::LoadFontData(char* filename)
{
	std::ifstream fin;
	char temp;

	// Create the font spacing buffer
	m_Font = new FontType[95];

	// Read in the font size and spacing between chars
	fin.open(filename);

	// Read in the 95 used ascii characters for text
	for(int i = 0; i < 95; ++i)
	{
		fin.get(temp);
		while(temp != ' ')
			fin.get(temp);

		fin.get(temp);
		while(temp != ' ')
			fin.get(temp);

		fin >> m_Font[i].left;
		fin >> m_Font[i].right;
		fin >> m_Font[i].size;
	}

	// Close the file
	fin.close();
}

void Font::BuildVertexArray(void* vertices, char* sentence, float drawX, float drawY)
{
	FontVertex* vertexPtr;
	int numLetters, index, letter;

	// Coerce the input vertices into a Vertex structure
	vertexPtr = (FontVertex*)vertices;

	// Get the number of letters in the sentence
	numLetters = (int)strlen(sentence);

	// Initialize the index to the vertex array
	index = 0;

	// Draw each letter onto a quad
	for(int i = 0; i < numLetters; ++i)
	{
		letter = ((int) sentence[i]) - 32;

		// If the letter is a space then just move over three pixels
		if(letter == 0)
			drawX = drawX + 3.0f;

		else
		{
			// The UV numbers are determined by the bit number they are divided by the total length of the image (1024)
			// 1/1024 for !

			// First triangle in quad
			vertexPtr[index].Position = XMFLOAT3(drawX, drawY, 0.0f);	// Top Left
			vertexPtr[index].UV = XMFLOAT2(m_Font[letter].left, 0.0f);
			index++;

			vertexPtr[index].Position = XMFLOAT3((drawX + (m_Font[letter].size)), (drawY - 16), 0.0f);  // Bottom right.
			vertexPtr[index].UV = XMFLOAT2(m_Font[letter].right, UV_HEIGHT);
			index++;

			vertexPtr[index].Position = XMFLOAT3(drawX, (drawY - 16), 0.0f);  // Bottom left.
			vertexPtr[index].UV = XMFLOAT2(m_Font[letter].left, UV_HEIGHT);
			index++;

			// Second triangle in quad.
			vertexPtr[index].Position = XMFLOAT3(drawX, drawY, 0.0f);  // Top left.
			vertexPtr[index].UV = XMFLOAT2(m_Font[letter].left, 0.0f);
			index++;

			vertexPtr[index].Position = XMFLOAT3(drawX + (m_Font[letter].size), drawY, 0.0f);  // Top right.
			vertexPtr[index].UV = XMFLOAT2(m_Font[letter].right, 0.0f);
			index++;

			vertexPtr[index].Position = XMFLOAT3((drawX + (m_Font[letter].size)), (drawY - 16), 0.0f);  // Bottom right.
			vertexPtr[index].UV = XMFLOAT2(m_Font[letter].right, UV_HEIGHT);
			index++;

			// Update the x location for drawing by the size of the letter and one pixel.
			drawX = drawX + m_Font[letter].size + 1.0f;
		}
	}
}