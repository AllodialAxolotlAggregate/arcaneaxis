#include "Sentence.h"
#define ReleaseMacro(x) { if(x){ x->Release(); x = 0; } }
#define IDENTITY_MATRIX XMFLOAT4X4(1.0, 0.0, 0.0, 0.0, 0.0, 1.0, 0.0, 1.0, 0.0, 0.0, 1.0, 0.0, 0.0, 0.0, 0.0, 1.0)

using namespace DirectX;

Sentence::Sentence() :
	m_Font(nullptr),
	m_FontShader(nullptr),
	m_VertexBuffer(nullptr),
	m_IndexBuffer(nullptr),
	m_VertexCount(0),
	m_IndexCount(0),
	m_Length(0),
	m_Color(XMFLOAT4(1.0, 0.0, 0.0, 0.0)),
	m_Position(XMFLOAT3(0.0, 0.0, 0.0)),
	m_WorldMatrix(IDENTITY_MATRIX),
	m_Device(nullptr),
	m_DeviceContext(nullptr)
{
}

Sentence::Sentence(ID3D11Device* device, ID3D11DeviceContext* deviceContext) :
	m_Font(nullptr),
	m_FontShader(nullptr),
	m_VertexBuffer(nullptr),
	m_IndexBuffer(nullptr),
	m_VertexCount(0),
	m_IndexCount(0),
	m_Length(0),
	m_Color(XMFLOAT4(1.0, 0.0, 0.0, 0.0)),
	m_Position(XMFLOAT3(0.0, 0.0, 0.0)),
	m_WorldMatrix(IDENTITY_MATRIX),
	m_Device(device),
	m_DeviceContext(deviceContext)
{
}

Sentence::~Sentence()
{
	this->Release();
}

void Sentence::LoadFontAndShader(Font* aFont, FontShader* aFontShader)
{
	m_Font = aFont;
	m_FontShader = aFontShader;
}

void Sentence::Initialize(char* text, int positionX, int positionY)
{
	//if(*m_Sentence != *text)
	//{
#pragma region Text - InitializeSetence Method

		FontVertex* vertices;
		unsigned long* indices;
		D3D11_BUFFER_DESC vertexBufferDesc, indexBufferDesc;
		D3D11_SUBRESOURCE_DATA vertexData, indexData;

		m_Position = XMFLOAT3(positionX, positionY, 0.0);

		// Set the counts of the vertices and indices
		m_Length = strlen(text);
		m_VertexCount = 6 * m_Length;
		m_IndexCount = m_VertexCount;

		// Create the vertex and index arrays.
		vertices = new FontVertex[m_VertexCount];
		indices = new unsigned long[m_IndexCount];

		// Initialize vertex array to zeros at first.
		memset(vertices, 0, (sizeof(FontVertex) * m_VertexCount));

		// Initialize the index array.
		for(int i = 0; i < m_IndexCount; i++)
			indices[i] = i;

		#pragma region Buffer Descriptions

		// Set up the description of the dynamic vertex buffer.
		vertexBufferDesc.Usage = D3D11_USAGE_DYNAMIC;
		vertexBufferDesc.ByteWidth = sizeof(FontVertex) * m_VertexCount;
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
		indexBufferDesc.ByteWidth = sizeof(unsigned long) * m_IndexCount;
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
		m_Device->CreateBuffer(&vertexBufferDesc, &vertexData, &m_VertexBuffer);
		m_Device->CreateBuffer(&indexBufferDesc, &indexData, &m_IndexBuffer);

		// Release the vertex array as it is no longer needed.
		delete [] vertices;
		vertices = nullptr;

		// Release the index array as it is no longer needed.
		delete [] indices;
		indices = nullptr;

#pragma endregion

#pragma region Text - UpdateSentence Method

		int numLetters;
		float drawX, drawY;
		D3D11_MAPPED_SUBRESOURCE mappedResource;
		FontVertex* verticesPtr;

		// Store the color of the sentence.
		m_Color = XMFLOAT4(0.0, 0.0, 1.0, 0.0);

		// Create the vertex array.
		vertices = new FontVertex[m_VertexCount];

		// Initialize vertex array to zeros at first.
		memset(vertices, 0, (sizeof(FontVertex) * m_VertexCount));

		// Calculate the X and Y pixel position on the screen to start drawing to.
		/*drawX = positionX;
		drawY = positionY;*/

		// Use the font class to build the vertex array from the sentence text and sentence draw location.
		m_Font->BuildVertexArray((void*)vertices, text, m_Position.x, m_Position.y);

		// Lock the vertex buffer so it can be written to.
		m_DeviceContext->Map(m_VertexBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedResource);

		// Get a pointer to the data in the vertex buffer.
		verticesPtr = (FontVertex*)mappedResource.pData;

		// Copy the data into the vertex buffer.
		memcpy(verticesPtr, (void*)vertices, (sizeof(FontVertex) * m_VertexCount));

		// Unlock the vertex buffer.
		m_DeviceContext->Unmap(m_VertexBuffer, 0);

		// Release the vertex array as it is no longer needed.
		delete [] vertices;
		vertices = nullptr;

#pragma endregion
	//}
}

void Sentence::Render(DirectX::XMFLOAT4X4 viewMatrix, DirectX::XMFLOAT4X4 projectionMatrix)
{
	unsigned int stride, offset;
	XMFLOAT4 pixelColor;
	bool result;

	// Set vertex buffer stride and offset.
	stride = sizeof(FontVertex); 
	offset = 0;

	// Set the vertex buffer to active in the input assembler so it can be rendered.
	m_DeviceContext->IASetVertexBuffers(0, 1, &m_VertexBuffer, &stride, &offset);

    // Set the index buffer to active in the input assembler so it can be rendered.
	m_DeviceContext->IASetIndexBuffer(m_IndexBuffer, DXGI_FORMAT_R32_UINT, 0);

    // Set the type of primitive that should be rendered from this vertex buffer, in this case triangles.
	m_DeviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

	// Create a pixel color vector with the input sentence color.
	m_Color = XMFLOAT4(1.0, 0.0, 1.0, 1.0);

	// Render the text using the font shader.
	m_FontShader->Render(m_DeviceContext, m_IndexCount, m_WorldMatrix, viewMatrix, projectionMatrix, m_Font->r_ShaderResourceView, m_Color);
}

void Sentence::Release()
{
	ReleaseMacro(m_VertexBuffer);
	ReleaseMacro(m_IndexBuffer);
	ReleaseMacro(m_Device);
	ReleaseMacro(m_DeviceContext);

	m_VertexCount = 0;
	m_IndexCount = 0;
	m_Length = 0;
	m_Color = XMFLOAT4(0.0, 0.0, 0.0, 0.0);
	m_Position = XMFLOAT3(0.0, 0.0, 0.0);

	/*if(m_Font != nullptr)
		delete m_Font;

	if(m_FontShader != nullptr)
		delete m_FontShader;*/
}

void Sentence::WorldTransition()
{
	XMMATRIX trans = XMMatrixTranslation(m_Position.x, m_Position.y, m_Position.z);
	/*XMMATRIX rotX = XMMatrixRotationY(m_Rotation.x);
	XMMATRIX rotY = XMMatrixRotationY(m_Rotation.y);
	XMMATRIX rotZ = XMMatrixRotationY(m_Rotation.z);
	XMMATRIX scale = XMMatrixScaling(m_Scale.x, m_Scale.y, m_Scale.z);*/

	//XMMATRIX w = scale * rotX * rotY * rotZ * trans;

	XMStoreFloat4x4(&m_WorldMatrix, XMMatrixTranspose(trans));
}