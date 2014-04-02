#include "Font.h"

using namespace DirectX;

Font::Font() : m_Font(0), m_ShaderResourceView(0) {}
//Font::Font() : m_Font(0) {}
Font::~Font() {}

void Font::Initialize(ID3D11Device* device,ID3D11DeviceContext* deviceContext, char* fontFilename, WCHAR* textureFilename)
{
	// Load in the text file containing the font data
	LoadFontData(fontFilename);
	CreateWICTextureFromFile(device, deviceContext, textureFilename, 0, &m_ShaderResourceView);
}

//void Font::Initialize(char* fontFilename)
//{
//	// Load in the text file containing the font data
//	LoadFontData(fontFilename);
//}

void Font::Shutdown()
{
	ReleaseFontData();
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

void Font::ReleaseFontData()
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

void Font::BuildVertexArray(void* vertices, char* sentence, float drawX, float drawY)
{
	VertexType* vertexPtr;
	int numLetters, index, letter;

	// Coerce the input vertices into a VertexType structure
	vertexPtr = (VertexType*)vertices;

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
			// First triangle in quad
			vertexPtr[index].position = XMFLOAT3(drawX, drawY, 0.0f);	// Top Left
			vertexPtr[index].texture = XMFLOAT2(m_Font[letter].left, 0.0f);
			index++;

			vertexPtr[index].position = XMFLOAT3((drawX + m_Font[letter].size), (drawY - 16), 0.0f);  // Bottom right.
			vertexPtr[index].texture = XMFLOAT2(m_Font[letter].right, 1.0f);
			index++;

			vertexPtr[index].position = XMFLOAT3(drawX, (drawY - 16), 0.0f);  // Bottom left.
			vertexPtr[index].texture = XMFLOAT2(m_Font[letter].left, 1.0f);
			index++;

			// Second triangle in quad.
			vertexPtr[index].position = XMFLOAT3(drawX, drawY, 0.0f);  // Top left.
			vertexPtr[index].texture = XMFLOAT2(m_Font[letter].left, 0.0f);
			index++;

			vertexPtr[index].position = XMFLOAT3(drawX + m_Font[letter].size, drawY, 0.0f);  // Top right.
			vertexPtr[index].texture = XMFLOAT2(m_Font[letter].right, 0.0f);
			index++;

			vertexPtr[index].position = XMFLOAT3((drawX + m_Font[letter].size), (drawY - 16), 0.0f);  // Bottom right.
			vertexPtr[index].texture = XMFLOAT2(m_Font[letter].right, 1.0f);
			index++;

			// Update the x location for drawing by the size of the letter and one pixel.
			drawX = drawX + m_Font[letter].size + 1.0f;
		}
	}
}