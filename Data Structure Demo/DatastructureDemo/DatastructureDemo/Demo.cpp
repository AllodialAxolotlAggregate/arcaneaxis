#include "Demo.h"


Demo::Demo(void)
{
}


Demo::~Demo(void)
{
}

int main()
{
	std::cout<<"HELLO WORLD I GUESS" << std::endl;

	// Create spellbook
	Spellbook* sp = new Spellbook();
	sp->CreateSpell( new Rune(100), new Rune(200), new Rune(300) );

	sp->Print();
	getchar();
	return 0;
}