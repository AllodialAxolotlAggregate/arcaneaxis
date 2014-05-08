#pragma once
#include <d3d11.h>
#include "Vertex.h"
/* This class describes the geometrical area that a rune will be displayed in and its relationships
 * to the world and the artifact that it is contained inside. */

/* The idea for this is to be a member of the Artifact and inherit most positions from that, 
 * and have them updated from the Artifact class. */

class Tile
{
public:
	Tile(Vertex* _v1, Vertex* _v2, Vertex* _v3)
	{
		m_tileVerts = new Vertex[3];
		m_tileVerts[0] = *_v1;
		m_tileVerts[1] = *_v2;
		m_tileVerts[2] = *_v3;
	}

	Tile()
	{
		this->m_tileVerts = nullptr;
	}

	~Tile(void)
	{
		if(m_tileVerts != nullptr)
		{
			delete[] m_tileVerts;
			m_tileVerts = nullptr;
		}
	}

	
private: 
	Vertex* m_tileVerts;

	void GenerateDisplayable()
	{
	}

	// material for texture

	// and/or game entity for tile

	// center

	// radii and/or vertices

	// matrix

	//method to check area input and see if it collides

	// method to check screen area and see if it collides

	// get image

	// render image

	// change image

	// clicked y/n

	
};

