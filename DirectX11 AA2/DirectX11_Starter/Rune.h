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

	//const int MAX_VAL = 10;
	//const int  MIN_VAL = 0;
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
		/*
		// Sanitize integer input
		if(_int > MAX_VAL || _int > MIN_VAL )
		{
			_int = MIN_VAL;
		}
		else*/ m_int = _int;
		

		// TODO: set texture path to actual rune texture dynamically instead of for demo
		m_texturePath = "images/crow.png";  // for starters

		// Runes will be given a texture path based on their integer values (for now)
	}

	~Rune(void);


private: 
	runeStyle m_style;
	int m_int;
	char* m_texturePath;

};

#endif