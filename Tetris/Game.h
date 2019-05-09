//Class for the main game loop. Also stores constants for Playfield, Player, and UI.
#pragma once
#ifndef GAME_H
#define GAME_H

#include <SDL.h>
#include "Playfield.h"
#include "Player.h"
#include "UI.h"

class Game {
public:
	Game();
	~Game();
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