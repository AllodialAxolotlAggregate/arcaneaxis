#include "MyText.h"

#define IDENTITY_MATRIX XMFLOAT4X4(1.0, 0.0, 0.0, 0.0, 0.0, 1.0, 0.0, 1.0, 0.0, 0.0, 1.0, 0.0, 0.0, 0.0, 0.0, 1.0)

using namespace DirectX;

MyText::MyText() :
	m_Mesh(nullptr),
	m_Material(nullptr),
	m_WorldMatrix(IDENTITY_MATRIX)
{
	WorldTransition();
}

MyText::~MyText()
{
}

void MyText::WorldTransition()
{
	XMMATRIX trans = XMMatrixTranslation(m_Position.x, m_Position.y, m_Position.z);
	XMStoreFloat4x4(&m_WorldMatrix, XMMatrixTranspose(trans));
}

void MyText::Draw(Camera* aCamera)
{
	material->Draw(aCamera);
	mesh->Draw();
}

void MyText::BuildVertexArray(void* vertices, char* sentence, float drawX, float drawY)
{
	Vertex* vertexPtr;
	int numLetters, index, letter;

	// Coerce the input vertices into a Vertex structure
	vertexPtr = (Vertex*)vertices;

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
			vertexPtr[index].Position = XMFLOAT3(drawX, drawY, 0.0f);	// Top Left
			vertexPtr[index].UV = XMFLOAT2(m_Font[letter].left, 0.0f);
			index++;

			vertexPtr[index].Position = XMFLOAT3((drawX + m_Font[letter].size), (drawY - 16), 0.0f);  // Bottom right.
			vertexPtr[index].UV = XMFLOAT2(m_Font[letter].right, 1.0f);
			index++;

			vertexPtr[index].Position = XMFLOAT3(drawX, (drawY - 16), 0.0f);  // Bottom left.
			vertexPtr[index].UV = XMFLOAT2(m_Font[letter].left, 1.0f);
			index++;

			// Second triangle in quad.
			vertexPtr[index].Position = XMFLOAT3(drawX, drawY, 0.0f);  // Top left.
			vertexPtr[index].UV = XMFLOAT2(m_Font[letter].left, 0.0f);
			index++;

			vertexPtr[index].Position = XMFLOAT3(drawX + m_Font[letter].size, drawY, 0.0f);  // Top right.
			vertexPtr[index].UV = XMFLOAT2(m_Font[letter].right, 0.0f);
			index++;

			vertexPtr[index].Position = XMFLOAT3((drawX + m_Font[letter].size), (drawY - 16), 0.0f);  // Bottom right.
			vertexPtr[index].UV = XMFLOAT2(m_Font[letter].right, 1.0f);
			index++;

			// Update the x location for drawing by the size of the letter and one pixel.
			drawX = drawX + m_Font[letter].size + 1.0f;
		}
	}
}