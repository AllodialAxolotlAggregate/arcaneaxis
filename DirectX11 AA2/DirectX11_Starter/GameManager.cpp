#include "GameManager.h"


GameManager::GameManager(){
	gameState = game;
}

char* GameManager::GetStateString(){
	switch(gameState)
	{
		case game:
			return "Game";
		case pause:
			return "Paused";
		
	}
}