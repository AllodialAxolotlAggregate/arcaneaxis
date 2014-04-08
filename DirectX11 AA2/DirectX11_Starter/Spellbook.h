#ifndef SPELLBOOK_H
#define SPELLBOOK_H

#include "SuperVector.h"
#include "Rune.h"
/* Spellbooks describe the collection of spells, which are groups of runes. */

class Spellbook
{

private: 
	// A pointer to a vector of a pointer to a vector filled with pointers to runes. 
	SuperVector< SuperVector<Rune*>* >* m_spellbook;

	SuperVector<Rune*>* m_activeSpell;

public:
	// Constructor for a blank Spellbook
	Spellbook(void)
	{
		// Allocate for our super SuperVector pointer on the heap (MEGA VECTOOOORR)
		m_spellbook = new SuperVector< SuperVector<Rune*>* >();

		// Allocate for our SuperVector that will account for active spells
		m_activeSpell = new SuperVector<Rune*>();
	};

	// To be called after a certain amount of time of no new entries
	void ClearActiveSpell()
	{
		//m_activeSpell->
	}


	// Add a spell to the spellbook. To be used at the beginning of levels. 
	void AddSpell( SuperVector<Rune*>* _ra)
	{
		this->m_spellbook->Push(_ra);
	}

	// Pass in a certain number of rune pointers ... and get what you need for AddSpell(). 
	// NOTE: Overload this method to be able to create spells with combos in numbers other than 3. 
	SuperVector<Rune*>* CreateSpell( Rune* r1, Rune* r2, Rune* r3)
	{
		std::cout<< *r1 << std::endl;
		
		SuperVector<Rune*>* temp = new SuperVector<Rune*>();

		temp->Push(r1);
		temp->Push(r2);
		temp->Push(r3);

		// Double-check with text
		for(int i = 0; i < temp->GetSize(); i++)
		{
			std::cout<< *temp->RetrieveByIndex(i) << "}" << std::endl;
		}

		return temp;
	}

	// Destructor 
	~Spellbook(void);




	// For debugging
	void Print()
	{
		for(int i =0; i< m_spellbook->GetSize(); i++)
		{
			std::cout<< "[";
			// For each spell that consists of runes,
			for(int j =0; j < 2; j++)
			{
				std::cout<<"{"<< i << ", " << j << "}";
				// For each rune, print it.
				std::cout << m_spellbook->RetrieveByIndex(i)->RetrieveByIndex(j) << ", ";
			}
			std::cout<<"]";
		}

	}

};

#endif