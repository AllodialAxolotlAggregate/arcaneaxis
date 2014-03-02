#include "Mesh.h"
#define delete_s(x) if(x != null && x != nullptr) delete(x)

using namespace DirectX;

Mesh::Mesh()
{
	m_NumberOfVertices = 0;
	m_NumberOfIndices = 0;
}

Mesh::Mesh(Vertex *verts, UINT *inds, UINT numVerts, UINT numInds, ID3D11Device* device)
{
	m_NumberOfIndices = numInds;
	m_NumberOfVertices = numVerts;

	// Create the vertex buffer
	D3D11_BUFFER_DESC m_vbd;
    m_vbd.Usage					= D3D11_USAGE_IMMUTABLE;
	m_vbd.ByteWidth				= sizeof(Vertex) * numVerts; // Number of vertices
    m_vbd.BindFlags				= D3D11_BIND_VERTEX_BUFFER;
    m_vbd.CPUAccessFlags		= 0;
    m_vbd.MiscFlags				= 0;
	m_vbd.StructureByteStride	= 0;

	D3D11_SUBRESOURCE_DATA m_InitialVertexData;
	m_InitialVertexData.pSysMem	= verts;

	device->CreateBuffer(&m_vbd, &m_InitialVertexData, &m_VertexBuffer);

	// Create the index buffer
	D3D11_BUFFER_DESC m_ibd;
    m_ibd.Usage					= D3D11_USAGE_IMMUTABLE;
	m_ibd.ByteWidth				= sizeof(UINT) * numInds; // Number of indices
    m_ibd.BindFlags				= D3D11_BIND_INDEX_BUFFER;
    m_ibd.CPUAccessFlags		= 0;
    m_ibd.MiscFlags				= 0;
	m_ibd.StructureByteStride	= 0;

	D3D11_SUBRESOURCE_DATA m_InitialIndexData;
	m_InitialIndexData.pSysMem	= inds;

	device->CreateBuffer(&m_ibd, &m_InitialIndexData, &m_IndexBuffer);
}

Mesh::~Mesh() {}

int Mesh::GetNumberOfIndices() { return m_NumberOfIndices; }
void Mesh::SetNumberOfIndices(int number) { m_NumberOfIndices = number; }

int Mesh::GetNumberOfVertices() { return m_NumberOfVertices; }
void Mesh::SetNumberOfVertices(int number) { m_NumberOfVertices = number; }

ID3D11Buffer *Mesh::GetVertexBuffer() { return m_VertexBuffer; }
void Mesh::SetVertexBuffer(ID3D11Buffer *m_nVBuffer) { m_VertexBuffer = m_nVBuffer; }

ID3D11Buffer *Mesh::GetIndexBuffer() { return m_IndexBuffer; }
void Mesh::SetIndexBuffer(ID3D11Buffer *m_nIBuffer) { m_IndexBuffer = m_nIBuffer; }