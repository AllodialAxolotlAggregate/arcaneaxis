#include "GameManager.h"

GameManager::GameManager(){
	gameState = game;
	debug = true;
}

char* GameManager::GetStateString(){
	switch(gameState)
	{
	case title: return "Title";
		case game:
			return "Game";
		case pause:
			return "Paused";
		case gameOver : return "Game Over";
		
	}
}