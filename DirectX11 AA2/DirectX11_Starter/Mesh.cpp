#include "Mesh.h"
#define delete_s(x) if(x != NULL && x != nullptr) delete(x)

using namespace DirectX;

Mesh::Mesh() :
	m_NumberOfVertices(0),
	m_NumberOfIndices(0),
	m_NumberOfFaces(0),
	m_VertexBuffer(nullptr),
	m_IndexBuffer(nullptr),
	m_Device(nullptr),
	m_DeviceContext(nullptr),
	m_Vertices(nullptr),
	m_Indices(nullptr),
	m_Faces(nullptr),
	m_OriginalVertices(nullptr)
{
}

Mesh::Mesh(ID3D11Device* device, ID3D11DeviceContext* deviceContext) :
	m_NumberOfVertices(0),
	m_NumberOfIndices(0),
	m_NumberOfFaces(0),
	m_VertexBuffer(nullptr),
	m_IndexBuffer(nullptr),
	m_Device(device),
	m_DeviceContext(deviceContext),
	m_Vertices(nullptr),
	m_Indices(nullptr),
	m_Faces(nullptr),
	m_OriginalVertices(nullptr)
{
}

Mesh::~Mesh()
{
	m_NumberOfVertices = 0;
	m_NumberOfIndices = 0;
	m_NumberOfFaces = 0;

	if(m_Vertices != nullptr)
	{
		delete[] m_Vertices;
		m_Vertices = nullptr;
	}

	if(m_Indices != nullptr)
	{
		delete[] m_Indices;
		m_Indices = nullptr;
	}

	if(m_Faces != nullptr)
	{
		delete[] m_Faces;
		m_Faces = nullptr;
	}

	if(m_Device != nullptr)
		m_Device= nullptr;

	if(m_DeviceContext != nullptr)
		m_DeviceContext = nullptr;

	if(m_OriginalVertices != nullptr)
	{
		delete[] m_OriginalVertices;
		m_OriginalVertices = nullptr;
	}
}

void Mesh::LoadNumbers(UINT numberOfVertices, UINT numberOfIndices)
{
	m_NumberOfVertices = numberOfVertices;
	m_NumberOfIndices = numberOfIndices;
	m_NumberOfFaces = numberOfIndices/3;
	m_Vertices = new Vertex[numberOfVertices];
	m_Indices = new UINT[numberOfIndices];
	m_Faces = new Face[m_NumberOfFaces];

	m_OriginalVertices = new Vertex[numberOfVertices];
}

void Mesh::Draw()
{
	UINT stride = sizeof(Vertex);
	UINT offset = 0;

	m_DeviceContext->IASetVertexBuffers(0, 1, &m_VertexBuffer, &stride, &offset);
	m_DeviceContext->IASetIndexBuffer(m_IndexBuffer, DXGI_FORMAT_R32_UINT, 0);

	m_DeviceContext->DrawIndexed(m_NumberOfIndices, 0, 0);
}

void Mesh::LoadBuffers(Vertex* vertices, UINT* indices)
{
	// for Tile purposes
	for(int i = 0; i < m_NumberOfVertices; ++i)
	{
		m_Vertices[i] = vertices[i];
		m_OriginalVertices[i] = vertices[i];
	}

	for(int j = 0; j < m_NumberOfIndices; ++j)
		m_Indices[j] = indices[j];

	int num, vertNumber, triangle = 0;
	for(triangle = 0; triangle < m_NumberOfFaces; ++triangle)
	{
		num = 3 * triangle;
		for(vertNumber = 0; vertNumber < 3; ++vertNumber)
		{
			//num += vertNumber;
			m_Faces[triangle].r_Indices[vertNumber] = indices[num];
			m_Faces[triangle].r_Vertices[vertNumber] = vertices[m_Faces[triangle].r_Indices[vertNumber]];
			++num;
		}
	}



	D3D11_BUFFER_DESC m_vbd;
    m_vbd.Usage					= D3D11_USAGE_IMMUTABLE;
	m_vbd.ByteWidth				= sizeof(Vertex) * m_NumberOfVertices; // Number of vertices
    m_vbd.BindFlags				= D3D11_BIND_VERTEX_BUFFER;
    m_vbd.CPUAccessFlags		= 0;
    m_vbd.MiscFlags				= 0;
	m_vbd.StructureByteStride	= 0;

	D3D11_SUBRESOURCE_DATA m_InitialVertexData;
	m_InitialVertexData.pSysMem	= m_Vertices;

	m_Device->CreateBuffer(&m_vbd, &m_InitialVertexData, &m_VertexBuffer);

	// Create the index buffer
	D3D11_BUFFER_DESC m_ibd;
    m_ibd.Usage					= D3D11_USAGE_IMMUTABLE;
	m_ibd.ByteWidth				= sizeof(UINT) * m_NumberOfIndices; // Number of indices
    m_ibd.BindFlags				= D3D11_BIND_INDEX_BUFFER;
    m_ibd.CPUAccessFlags		= 0;
    m_ibd.MiscFlags				= 0;
	m_ibd.StructureByteStride	= 0;

	D3D11_SUBRESOURCE_DATA m_InitialIndexData;
	m_InitialIndexData.pSysMem	= m_Indices;

	m_Device->CreateBuffer(&m_ibd, &m_InitialIndexData, &m_IndexBuffer);
}