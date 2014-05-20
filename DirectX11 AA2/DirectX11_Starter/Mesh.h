#ifndef MESH_H
#define MESH_H

#include <d3d11.h>
#include <DirectXMath.h>
#include <fstream>
#include "Face.h"

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
	//		DirectX11 Device Ponter/Address,
	Mesh(ID3D11Device* device, ID3D11DeviceContext* deviceContext);
	
	// The Destructor - currently does nothing
	~Mesh();

	// Sends the vertice and indice information to the Graphics to draw on screen
	//		Needs a DirectX11 DeviceContext Pointer/Address
	void Draw();

	void LoadBuffers(Vertex* vertices, UINT* indices);
	void LoadNumbers(UINT numberOfVertices, UINT numberOfIndices);

	void LoadVertices(Vertex* vertices, UINT* indices);
	void CreateBuffers();

#pragma region Gets/Sets

	// http://stackoverflow.com/questions/2284610/what-is-declspec-and-when-do-i-need-to-use-it
	void SetNumberOfIndices(int number) { m_NumberOfIndices = number; }
	int GetNumberOfIndices() { return m_NumberOfIndices; }
	__declspec(property(get = GetNumberOfIndices, put = SetNumberOfIndices)) int r_NumberOfIndices;

	void SetNumberOfVertices(int number) { m_NumberOfVertices = number; }
	int GetNumberOfVertices() { return m_NumberOfVertices; }
	__declspec(property(get = GetNumberOfVertices, put = SetNumberOfVertices)) int r_NumberOfVertices;

	void SetNumberOfTriangles(int number) { m_NumberOfFaces = number; }
	int GetNumberOfTriangles() { return m_NumberOfFaces; }
	__declspec(property(get = GetNumberOfTriangles, put = SetNumberOfTriangles)) int r_NumberOfFaces;

	void SetVertexBuffer(ID3D11Buffer *otherVertexBuffer) { m_VertexBuffer = otherVertexBuffer; }
	ID3D11Buffer* GetVertexBuffer() { return m_VertexBuffer; }
	__declspec(property(get = GetVertexBuffer, put = SetVertexBuffer)) ID3D11Buffer* r_VertexBuffer;

	void SetIndexBuffer(ID3D11Buffer *otherIndexBuffer) { m_IndexBuffer = otherIndexBuffer; }
	ID3D11Buffer* GetIndexBuffer() { return m_IndexBuffer; }
	__declspec(property(get = GetIndexBuffer, put = SetIndexBuffer)) ID3D11Buffer* r_IndexBuffer;

	void SetDevice(ID3D11Device* otherDevice) { m_Device = otherDevice; }
	ID3D11Device* GetDevice() { return m_Device; }
	__declspec(property(get = GetDevice, put = SetDevice)) ID3D11Device* r_Device;

	void SetDeviceContext(ID3D11DeviceContext* otherDeviceContext) { m_DeviceContext = otherDeviceContext; }
	ID3D11DeviceContext* GetDeviceContext() { return m_DeviceContext; }
	__declspec(property(get = GetDeviceContext, put = SetDeviceContext)) ID3D11DeviceContext* r_DeviceContext;

	void SetVertices(Vertex* otherVertices) { m_Vertices = otherVertices; }
	Vertex* GetVertices() { return m_Vertices; }
	__declspec(property(get = GetVertices, put = SetVertices)) Vertex* r_Vertices;

	void SetIndices(UINT* otherIndices) { m_Indices = otherIndices; }
	UINT* GetIndices() { return m_Indices; }
	__declspec(property(get = GetIndices, put = SetIndices)) UINT* r_Indices;

	void SetFaces(Face* otherFaces) { m_Faces = otherFaces; }
	Face* GetFaces() { return m_Faces; }
	__declspec(property(get = GetFaces, put = SetFaces)) Face* r_Faces;

	void SetOriginalVertices(Vertex* otherVertices) { m_OriginalVertices = otherVertices; }
	Vertex* GetOriginalVertices() { return m_OriginalVertices; }
	__declspec(property(get = GetOriginalVertices, put = SetOriginalVertices)) Vertex* r_OriginalVertices;

#pragma endregion

private:
	int m_NumberOfVertices, m_NumberOfIndices, m_NumberOfFaces;
	ID3D11Buffer *m_VertexBuffer, *m_IndexBuffer;
	ID3D11Device *m_Device;
	ID3D11DeviceContext *m_DeviceContext;
	Vertex* m_Vertices;
	UINT* m_Indices;
	Face* m_Faces;
	Vertex* m_OriginalVertices;
};

#endif