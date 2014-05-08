#include "Face.h"

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