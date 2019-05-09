/*
Xtvo's Tetris:
My goal is to make a fully functional Tetris clone using SDL in C++.
The first finished version should have at least:
- Main menu
- Endless mode (aka Marathon) with dynamic speed levels and scoring.
Then I can start adding more stuff if I want.

What's next on the agenda?
- Ready 3 2 1 go
- Main menu
- Sound effects + music
- Customizable controls
- Turn off automatic key repeating
- High scores
- Work my way through the Tetris Guideline and make sure everything is implemented.
https://tetris.fandom.com/wiki/Tetris_Guideline

Known bugs:
- Line clear flash has inconsistent speed. Tied to framerate issues?
- Piece sometimes locks really quickly after landing. Same as Tetris 99, wtf! I have no idea what the cause is.
- For now, there is no distinction between a T-spin and a T-spin mini. Both are counted as regular T-spins.
- Points added for soft/hard dropping don't cap at 20/40. Would be an easy implement but doesn't seem like that big an issue.
*/

#include <SDL.h>
#include <SDL_image.h>
#include <SDL_ttf.h>
#include <stdio.h>
#include <iostream>
#include <string>
#include <cmath>
#include "Game.h"
#include "Texture.h"
#include "Playfield.h"
#include "Player.h"
#include "UI.h"

//Screen dimension constants
const int SCREEN_WIDTH = 672; //320 playfield, 16 border, 168 next blocks, 168 hold blocks?
const int SCREEN_HEIGHT = 720;

//Frame counters
int gCurFrame;
int gPrevFrame;

//Window and screen surface
SDL_Window* gWindow = NULL;
SDL_Renderer* gRenderer = NULL;

//Aliases for passing to playerMove function
enum KeyPresses {
	KEY_PRESS_UP,
	KEY_PRESS_DOWN,
	KEY_PRESS_LEFT,
	KEY_PRESS_RIGHT,
	KEY_PRESS_TOTAL
};

//Initialize SDL. Returns true if no errors
bool init() {
	if (SDL_Init(SDL_INIT_VIDEO) < 0) {
		std::cout << "SDL could not initialize! SDL_Error: " << SDL_GetError() << "\n";
		return false;
	}

	//Create window
	gWindow = SDL_CreateWindow("Xtvo's Tetris", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN);
	if (gWindow == NULL) {
		std::cout << "Window could not be created! SDL_Error: " << SDL_GetError() << "\n";
		return false;
	}

	//Create renderer for window
	gRenderer = SDL_CreateRenderer(gWindow, -1, SDL_RENDERER_ACCELERATED);
	if (gRenderer == NULL)
	{
		printf("Renderer could not be created! SDL Error: %s\n", SDL_GetError());
		return false;
	}

	//Initialize renderer color
	SDL_SetRenderDrawColor(gRenderer, 0xFF, 0xFF, 0xFF, 0xFF);

	//Initialize .png loading
	int imgFlags = IMG_INIT_PNG;
	if (!(IMG_Init(imgFlags) & imgFlags))
	{
		std::cout << "SDL_image could not initialize! SDL_image Error: " << IMG_GetError() << "\n";
		return false;
	}

	//Initialize SDL_ttf
	if (TTF_Init() == -1)
	{
		printf("SDL_ttf could not initialize! SDL_ttf Error: %s\n", TTF_GetError());
		return false;
	}

	//Return with no errors
	return true;
}

//Ends the program
void end() {
	SDL_DestroyRenderer(gRenderer);
	SDL_DestroyWindow(gWindow);
	gWindow = NULL;
	gRenderer = NULL;

	TTF_Quit();
	IMG_Quit();
	SDL_Quit();
}

int main(int argc, char* args[])
{
	//Initialize SDL and abort if it fails
	if (!init())
		exit(1);

	Game game;
	while (!game.quitting()) {
		game.update();
	}

	//End of program
	end();

	return 0;
}