#ifndef MESH_H
#define MESH_H

#include <d3d11.h>
#include <DirectXMath.h>
#include "Vertex.h"

typedef unsigned int UINT;

class Mesh
{
public:
	Mesh();
	Mesh(Vertex[], UINT[], UINT, UINT, ID3D11Device*);
	~Mesh();

#pragma region Gets/Sets

	// http://stackoverflow.com/questions/2284610/what-is-declspec-and-when-do-i-need-to-use-it
	void SetNumberOfIndices(int number);
	int GetNumberOfIndices();
	__declspec(property(get = GetNumberOfIndices, put = SetNumberOfIndices)) int Indices;

	void SetNumberOfVertices(int number);
	int GetNumberOfVertices();
	__declspec(property(get = GetNumberOfVertices, put = SetNumberOfVertices)) int number;

	void SetVertexBuffer(ID3D11Buffer *m_nVBuffer);
	ID3D11Buffer* GetVertexBuffer();
	__declspec(property(get = GetVertexBuffer, put = SetVertexBuffer)) ID3D11Buffer* VertexBuffer;

	void SetIndexBuffer(ID3D11Buffer *m_nIBuffer);
	ID3D11Buffer* GetIndexBuffer();
	__declspec(property(get = GetIndexBuffer, put = SetIndexBuffer)) ID3D11Buffer* IndexBuffer;

#pragma endregion

private:
	int m_NumberOfVertices;
	int m_NumberOfIndices;

	ID3D11Buffer *m_VertexBuffer;
	ID3D11Buffer *m_IndexBuffer;
};

#endif