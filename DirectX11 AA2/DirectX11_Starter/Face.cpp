#include "Face.h"

using namespace DirectX;

Face::Face() :
	m_Vertices(new Vertex[3]),
	m_Indices(new UINT[3])
{
}

Face::~Face()
{
	delete[] m_Vertices;
	m_Vertices = nullptr;
	delete[] m_Indices;
	m_Indices = nullptr;
}

bool Face::PointInFace(float x, float y)
{

	XMVECTOR p = XMVectorSet(x, y, 0.0, 0.0);

	XMVECTOR a = XMVectorSet(m_Vertices[0].Position.x, m_Vertices[0].Position.y, 0.0, 0.0);
	XMVECTOR b = XMVectorSet(m_Vertices[1].Position.x, m_Vertices[1].Position.y, 0.0, 0.0);
	XMVECTOR c = XMVectorSet(m_Vertices[2].Position.x, m_Vertices[2].Position.y, 0.0, 0.0);
	
	a -= p;
	b -= p;
	c -= p;

	XMVECTOR u = XMVector2Cross(b, c);
	XMVECTOR v = XMVector2Cross(c, a);
	XMVECTOR d = XMVector2Dot(u,v);

	if (XMVectorGetByIndex(d,0) < 0.0f)
		return false;

	XMVECTOR w = XMVector2Cross(a,b);
	XMVECTOR e = XMVector2Dot(u,w);

	if (XMVectorGetByIndex(e,0) < 0.0f)
		return false;

	return true;
}