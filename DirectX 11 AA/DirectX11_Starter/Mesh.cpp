#include "Mesh.h"
#define delete_s(x) if(x != NULL && x != nullptr) delete(x)

using namespace DirectX;

Mesh::Mesh() :
	m_NumberOfVertices(0),
	m_NumberOfIndices(0),
	m_VertexBuffer(nullptr),
	m_IndexBuffer(nullptr),
	m_Device(nullptr),
	m_DeviceContext(nullptr)
{
}

Mesh::Mesh(UINT numVerts, UINT numInds, ID3D11Device* device, ID3D11DeviceContext* deviceContext) :
	m_NumberOfVertices(numVerts),
	m_NumberOfIndices(numInds),
	m_VertexBuffer(nullptr),
	m_IndexBuffer(nullptr),
	m_Device(device),
	m_DeviceContext(deviceContext)
{
}

Mesh::~Mesh()
{
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
	// Create the vertex buffer
	D3D11_BUFFER_DESC m_vbd;
    m_vbd.Usage					= D3D11_USAGE_IMMUTABLE;
	m_vbd.ByteWidth				= sizeof(Vertex) * m_NumberOfVertices; // Number of vertices
    m_vbd.BindFlags				= D3D11_BIND_VERTEX_BUFFER;
    m_vbd.CPUAccessFlags		= 0;
    m_vbd.MiscFlags				= 0;
	m_vbd.StructureByteStride	= 0;

	D3D11_SUBRESOURCE_DATA m_InitialVertexData;
	m_InitialVertexData.pSysMem	= vertices;

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
	m_InitialIndexData.pSysMem	= indices;

	m_Device->CreateBuffer(&m_ibd, &m_InitialIndexData, &m_IndexBuffer);
}