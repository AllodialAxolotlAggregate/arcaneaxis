#ifndef FACE_H
#define FACE_H

#include "Vertex.h"
#include <d3d11.h>
typedef unsigned int UINT;

class Face
{
public:
	Face();
	~Face();

#pragma region Gets/Sets

	void SetVertices(Vertex* otherVertices) { m_Vertices = otherVertices; }
	Vertex* GetVertices() { return m_Vertices; }
	__declspec(property(get = GetVertices, put = SetVertices)) Vertex* r_Vertices;

	void SetIndices(UINT* otherIndices) { m_Indices = otherIndices; }
	UINT* GetIndices() { return m_Indices; }
	__declspec(property(get = GetIndices, put = SetIndices)) UINT* r_Indices;

	bool PointInFace(float x, float y);

#pragma endregion

private:
	Vertex* m_Vertices;
	UINT* m_Indices;
};

#endif