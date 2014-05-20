#ifndef RUNE_H
#define RUNE_H

#include <iostream>
#include "SuperVector.h"

using namespace std;

enum runeStyle
{
	EGYPTIAN, 
	MEDIEVAL, 
	SPACE
};

class Rune
{


public:

	friend ostream& operator<<(ostream& os, Rune& r)
	{
	   // stream obj's data into os
		os << "HI THIS IS RUNE " << r.m_int ;
	   return os;
	}

	// Constructor that takes an integer for data.
	Rune(int _int)
	{
		// Sanitize integer input
		if(_int > 5|| _int > 0 )
		{
			_int = 0;
		}
		else m_int = _int;
		
		// Runes will be given a texture path based on their integer values (for now)
		// Let's put a pin in that.
	}

	~Rune(void);


private: 
	int m_int;
	char* m_texturePath;

};

#endif