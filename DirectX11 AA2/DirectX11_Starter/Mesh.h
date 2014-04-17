#ifndef MESH_H
#define MESH_H

#include <d3d11.h>
#include <DirectXMath.h>
#include "Vertex.h"

typedef unsigned int UINT;

class Mesh
{
public:
	// Empty Mesh Constuctor
	Mesh();

	// Mesh holds the information of where vertices are located in local computer space
	// Needs:
	//		Vertex Pointer/Address to array of stored Vertices
	//		Unsigned Int Pointer/Address to array of stored Indices
	//		An Unsigned Int for the number of Vertices
	//		An Unsigned Int for the number of indices
	//		DirectX11 Device Pointer/Address,
	Mesh(Vertex* vertices, UINT* indices, UINT numberOfVertices, UINT numberOfIndices, ID3D11Device* device);
	
	// The Destructor - currently does nothing
	~Mesh();

	// Sends the vertice and indice information to the Graphics to draw on screen
	//		Needs a DirectX11 DeviceContext Pointer/Address
	void Draw(ID3D11DeviceContext* deviceContext);

#pragma region Gets/Sets

	// http://stackoverflow.com/questions/2284610/what-is-declspec-and-when-do-i-need-to-use-it
	void SetNumberOfIndices(int number);
	int GetNumberOfIndices();
	__declspec(property(get = GetNumberOfIndices, put = SetNumberOfIndices)) int r_NumberOfIndices;

	void SetNumberOfVertices(int number);
	int GetNumberOfVertices();
	__declspec(property(get = GetNumberOfVertices, put = SetNumberOfVertices)) int r_NumberOfVertices;

	void SetVertexBuffer(ID3D11Buffer *m_nVBuffer);
	ID3D11Buffer* GetVertexBuffer();
	__declspec(property(get = GetVertexBuffer, put = SetVertexBuffer)) ID3D11Buffer* r_VertexBuffer;

	void SetIndexBuffer(ID3D11Buffer *m_nIBuffer);
	ID3D11Buffer* GetIndexBuffer();
	__declspec(property(get = GetIndexBuffer, put = SetIndexBuffer)) ID3D11Buffer* r_IndexBuffer;

#pragma endregion

private:
	int m_NumberOfVertices;
	int m_NumberOfIndices;

	ID3D11Buffer *m_VertexBuffer;
	ID3D11Buffer *m_IndexBuffer;

	/* gmb9280 : for purposes of Tiles */
	Vertex** m_verts;
};

#endif