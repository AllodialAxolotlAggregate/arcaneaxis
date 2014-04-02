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

	Rune(int _int)
	{
		m_style = EGYPTIAN;
		m_int = _int;
	}

	~Rune(void);


private: 
	runeStyle m_style;
	int m_int;
};

#endif