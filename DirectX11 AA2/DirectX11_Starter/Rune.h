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
	// Friend operator to enable stream printing.
	friend ostream& operator<<(ostream& os, Rune& r)
	{
	   // stream obj's data into os
		os << "HI THIS IS RUNE " << r.m_int ;
	   return os;
	}

	// Constructor that takes an integer for data.
	Rune(int _int)
	{
		m_style = EGYPTIAN;
		m_int = _int;
		// TODO: set texture path
	}

	~Rune(void);


private: 
	runeStyle m_style;
	int m_int;
	char* m_texturePath;

};

#endif