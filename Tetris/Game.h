//Class for the main game loop.
#pragma once
#ifndef GAME_H
#define GAME_H

#include <SDL.h>

class Playfield;
class Player;
class UI;

class Game {
public:
	Game(); //just calls init
	~Game(); //just calls end
	//The reason I have these functions is so I can delete playfield, player, etc
	//without deleting the Game
	void init();
	void end();
	void update();
	bool quitting();
	int getCurFrame();

private:
	Playfield *playfield1;
	Player *player1;
	UI *ui1;

	int startTime; //SDL Tick value of when the game was started
	bool quit; //whether the player has quit the game or not (alt-f4 or x out)
	SDL_Event event; //keyboard input
	int curFrame; //current frame
	int prevFrame; //previous frame
};

#endif