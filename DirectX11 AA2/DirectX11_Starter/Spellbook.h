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

	SuperVector< SuperVector<Rune*>* >* m_possibleMatches;

public:
	// Constructor for a blank Spellbook
	Spellbook(void)
	{
		// Allocate for our super SuperVector pointer on the heap (MEGA VECTOOOORR)
		m_spellbook = new SuperVector< SuperVector<Rune*>* >();

		// Allocate for our list of possible matches on the heap
		m_possibleMatches  = new SuperVector<SuperVector<Rune*>*>();

		// Allocate for our SuperVector that will account for active spells
		m_activeSpell = new SuperVector<Rune*>();
	};

	// To be called after a certain amount of time of no new entries
	void ClearActiveSpell()
	{
		m_activeSpell->Clear();
	}

	// Add a rune to the active spell
	// Need to check against possible matches afterwards
	void CastRune(Rune* _newrune)
	{
		m_activeSpell->Push(_newrune);
	}


	// Depending on the active runes of our m_activeSpell, narrow down which spells in 
	// the spellbook it could possibly be
	void CalculatePossibleMatches()
	{
		// If there are no spells in our possibilities vector 
		// and we just put the first rune into our active spell
		// then we need to calculate this fresh from ALL of the spells in the spellbook
		for(int i = 0; i < m_spellbook->GetSize() - 1; i++)
		{
			// For each spell in the book... check the runes of the spell
			for(int j = 0; j < m_activeSpell->GetSize() - 1; j++)
			{
				// not done yet
			}
		
		}


		// If we already have spells in our possibilities vector and we just 
		// need to narrow it down, 

	}

	// Add a spell to the spellbook. To be used at the beginning of levels or game.
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