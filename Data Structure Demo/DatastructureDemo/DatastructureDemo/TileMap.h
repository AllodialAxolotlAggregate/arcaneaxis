#pragma once
/* This class is kind of an all-in-one: 
 * It holds our grid of tiles,
 * our array of runes,
 * and maps them to each other.
 * Made by Gabrielle Bennett 3/18/14.
 */
#include "Rune.h"
#include "Tile.h"

class TileMap
{
private: 
	Rune* m_runes; // a 1D array of pointers to runes

	Tile** m_tileGrid; // a 2d array of pointers to tiles

	int m_numRunes; // how many runes we have

	int m_numTiles; // how many tiles we have

public:
	// CONSTRUCTOR : initialize to nothing
	TileMap(void)
	{
		m_runes = nullptr;
		m_tileGrid = nullptr;
		m_numRunes = 0;
	}
	
	// Real CONSTRUCTOR : take in info about tiles, and rune set to use
	TileMap(const int tileL_, const int tileH_, runeStyle rs_)
	{
		// Make our 2D array of tiles based on length and height params
		m_tileGrid = new Tile*[tileL_][tileH_];
	}

	// DESTRUCTOR
	~TileMap(void)
	{
		if(this->m_runes != nullptr)
		{
			delete[] this->m_runes;
			this->m_runes = nullptr;
			this->m_numRunes = 0;
		}
	}

	// Given a tile position, return a reference to the rune
	Rune& GetRuneAtTilePosition(Tile* t)
	{

	}


};

