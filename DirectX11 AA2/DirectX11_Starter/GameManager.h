#ifndef GAMEMANAGER_H
#define GAMEMANAGER_H

enum GameState {
		title,
		game,
		pause,
		gameOver
	};

class GameManager
{
public:
	GameManager();
	GameState gameState;
	
private:
};

#endif // GAMEMANAGER_H