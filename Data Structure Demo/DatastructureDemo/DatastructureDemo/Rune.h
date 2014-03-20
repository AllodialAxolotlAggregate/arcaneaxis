#pragma once

enum runeStyle
{
	EGYPTIAN, 
	MEDIEVAL, 
	SPACE
};

class Rune
{
public:
	Rune(void)
	{
		m_style = EGYPTIAN;
	}
	~Rune(void);
private: 
	runeStyle m_style;
};

