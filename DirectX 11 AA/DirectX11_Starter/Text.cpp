#include "Text.h"

using namespace DirectX;

Text::Text()
{
	m_Font = 0;
	m_FontShader = 0;

	m_sentence1 = 0;
	m_sentence2 = 0;
}

Text::~Text() {}

void Text::Initialize(ID3D11Device* device, ID3D11DeviceContext* deviceContext, int screenWidth, int screenHeight, 
						   DirectX::XMFLOAT4X4 baseViewMatrix)
{
	// Store the screen width and height.
	m_ScreenWidth = screenWidth;
	m_ScreenHeight = screenHeight;

	// Store the base view matrix.
	//m_baseViewMatrix = baseViewMatrix;

	// Create the font object.
	m_Font = new Font;

	// Initialize the font object.
	m_Font->Initialize(device, deviceContext, "../Fonts/fontdata.txt", L"../Fonts/font.dds");

	// Create the font shader object.
	m_FontShader = new FontShader;

	// Initialize the font shader object.
	m_FontShader->Initialize(device, L"FontVertexShader.cso", L"FontPixelShader.cso");

	// Initialize the first sentence.
	InitializeSentence(&m_sentence1, 16, device);

	// Now update the sentence vertex buffer with the new string information.
	UpdateSentence(m_sentence1, "Hello", 0, 0, 0.0f, 1.0f, 0.0f, deviceContext);

	// Initialize the first sentence.
	InitializeSentence(&m_sentence2, 16, device);

	// Now update the sentence vertex buffer with the new string information.
	UpdateSentence(m_sentence2, "Goodbye", 0, 0, 1.0f, 0.0f, 0.0f, deviceContext);
}

void Text::Shutdown()
{
	ReleaseSentence(&m_sentence1);
	ReleaseSentence(&m_sentence2);

	// Release the font shader object.
	if(m_FontShader)
	{
		m_FontShader->Shutdown();
		delete m_FontShader;
		m_FontShader = 0;
	}

	// Release the font object.
	if(m_Font)
	{
		m_Font->Shutdown();
		delete m_Font;
		m_Font = 0;
	}
}

void Text::Render(ID3D11DeviceContext* deviceContext, DirectX::XMFLOAT4X4 worldMatrix, DirectX::XMFLOAT4X4 projectionMatrix, DirectX::XMFLOAT4X4 orthoMatrix)
{
	// Currently rendering the text
	RenderSentence(deviceContext, m_sentence1, worldMatrix, projectionMatrix, orthoMatrix);
	// Rendering the color behind the text
	RenderSentence(deviceContext, m_sentence2, worldMatrix, projectionMatrix, orthoMatrix);
}

void Text::InitializeSentence(SentenceType** sentence, int maxLength, ID3D11Device* device)
{
	Vertex* vertices;
	unsigned long* indices;
	D3D11_BUFFER_DESC vertexBufferDesc, indexBufferDesc;
    D3D11_SUBRESOURCE_DATA vertexData, indexData;
	int i;

	// Create a new sentence object.
	*sentence = new SentenceType;

	// Initialize the sentence buffers to null.
	(*sentence)->vertexBuffer = nullptr;
	(*sentence)->indexBuffer = nullptr;

	// Set the maximum length of the sentence.
	(*sentence)->maxLength = maxLength;

	// Set the number of vertices in the vertex array.
	(*sentence)->vertexCount = 6 * maxLength;

	// Set the number of indexes in the index array.
	(*sentence)->indexCount = (*sentence)->vertexCount;

	// Create the vertex array.
	vertices = new Vertex[(*sentence)->vertexCount];

	// Create the index array.
	indices = new unsigned long[(*sentence)->indexCount];

	// Initialize vertex array to zeros at first.
	memset(vertices, 0, (sizeof(Vertex) * (*sentence)->vertexCount));

	// Initialize the index array.
	for(i=0; i<(*sentence)->indexCount; i++)
	{
		indices[i] = i;
	}

#pragma region Buffer Descriptions

	// Set up the description of the dynamic vertex buffer.
    vertexBufferDesc.Usage = D3D11_USAGE_DYNAMIC;
    vertexBufferDesc.ByteWidth = sizeof(Vertex) * (*sentence)->vertexCount;
    vertexBufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
    vertexBufferDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
    vertexBufferDesc.MiscFlags = 0;
	vertexBufferDesc.StructureByteStride = 0;

	// Give the subresource structure a pointer to the vertex data.
    vertexData.pSysMem = vertices;
	vertexData.SysMemPitch = 0;
	vertexData.SysMemSlicePitch = 0;

	// Set up the description of the static index buffer.
    indexBufferDesc.Usage = D3D11_USAGE_DEFAULT;
    indexBufferDesc.ByteWidth = sizeof(unsigned long) * (*sentence)->indexCount;
    indexBufferDesc.BindFlags = D3D11_BIND_INDEX_BUFFER;
    indexBufferDesc.CPUAccessFlags = 0;
    indexBufferDesc.MiscFlags = 0;
	indexBufferDesc.StructureByteStride = 0;

	// Give the subresource structure a pointer to the index data.
    indexData.pSysMem = indices;
	indexData.SysMemPitch = 0;
	indexData.SysMemSlicePitch = 0;

#pragma endregion

	// Create the vertex buffer and index buffer
    device->CreateBuffer(&vertexBufferDesc, &vertexData, &(*sentence)->vertexBuffer);
	device->CreateBuffer(&indexBufferDesc, &indexData, &(*sentence)->indexBuffer);

	// Release the vertex array as it is no longer needed.
	delete [] vertices;
	vertices = 0;

	// Release the index array as it is no longer needed.
	delete [] indices;
	indices = 0;
}

void Text::UpdateSentence(SentenceType* sentence, char* text, int positionX, int positionY, float red, float green, float blue,
							   ID3D11DeviceContext* deviceContext)
{
	int numLetters;
	Vertex* vertices;
	float drawX, drawY;
	D3D11_MAPPED_SUBRESOURCE mappedResource;
	Vertex* verticesPtr;

	// Store the color of the sentence.
	sentence->red = red;
	sentence->green = green;
	sentence->blue = blue;

	// Get the number of letters in the sentence.
	numLetters = (int)strlen(text);

	// Create the vertex array.
	vertices = new Vertex[sentence->vertexCount];

	// Initialize vertex array to zeros at first.
	memset(vertices, 0, (sizeof(Vertex) * sentence->vertexCount));

	// Calculate the X and Y pixel position on the screen to start drawing to.
	drawX = (float)(((m_ScreenWidth / 2) * -1) + positionX);
	drawY = (float)((m_ScreenHeight / 2) - positionY);
	/*drawX = (float)(positionX);
	drawY = (float)(positionY);*/

	// Use the font class to build the vertex array from the sentence text and sentence draw location.
	m_Font->BuildVertexArray((void*)vertices, text, drawX, drawY);

	// Lock the vertex buffer so it can be written to.
	deviceContext->Map(sentence->vertexBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedResource);

	// Get a pointer to the data in the vertex buffer.
	verticesPtr = (Vertex*)mappedResource.pData;

	// Copy the data into the vertex buffer.
	memcpy(verticesPtr, (void*)vertices, (sizeof(Vertex) * sentence->vertexCount));

	// Unlock the vertex buffer.
	deviceContext->Unmap(sentence->vertexBuffer, 0);

	// Release the vertex array as it is no longer needed.
	delete [] vertices;
	vertices = 0;
}

void Text::ReleaseSentence(SentenceType** sentence)
{
	if(*sentence)
	{
		// Release the sentence vertex buffer.
		if((*sentence)->vertexBuffer)
		{
			(*sentence)->vertexBuffer->Release();
			(*sentence)->vertexBuffer = 0;
		}

		// Release the sentence index buffer.
		if((*sentence)->indexBuffer)
		{
			(*sentence)->indexBuffer->Release();
			(*sentence)->indexBuffer = 0;
		}

		// Release the sentence.
		delete *sentence;
		*sentence = 0;
	}

	return;
}

void Text::RenderSentence(ID3D11DeviceContext* deviceContext, SentenceType* sentence, XMFLOAT4X4 worldMatrix, DirectX::XMFLOAT4X4 projectionMatrix, 
							   XMFLOAT4X4 orthoMatrix)
{
	unsigned int stride, offset;
	XMFLOAT4 pixelColor;
	bool result;

	// Set vertex buffer stride and offset.
    stride = sizeof(Vertex); 
	offset = 0;

	// Set the vertex buffer to active in the input assembler so it can be rendered.
	deviceContext->IASetVertexBuffers(0, 1, &sentence->vertexBuffer, &stride, &offset);

    // Set the index buffer to active in the input assembler so it can be rendered.
	deviceContext->IASetIndexBuffer(sentence->indexBuffer, DXGI_FORMAT_R32_UINT, 0);

    // Set the type of primitive that should be rendered from this vertex buffer, in this case triangles.
	//deviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

	// Create a pixel color vector with the input sentence color.
	pixelColor = XMFLOAT4(sentence->red, sentence->green, sentence->blue, 1.0f);

	// Render the text using the font shader.
	m_FontShader->Render(deviceContext, sentence->indexCount, worldMatrix, projectionMatrix, orthoMatrix, m_Font->r_ShaderResourceView, 
								  pixelColor);
}