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
		this->m_tileVerts = new Vertex*();
		this->m_tileVerts[0] = _v1;
		this->m_tileVerts[1] = _v2;
		this->m_tileVerts[2] = _v3;
	}

	~Tile(void);

	
private: 
	Vertex** m_tileVerts;
	// center

	// radii 

	//method to check area input and see if it collides

	// method to check screen area and see if it collides

	// get image

	// render image

	// change image

	// clicked

	
};

