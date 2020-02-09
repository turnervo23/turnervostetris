/*
Turner Vo's Tetris:
This program is a fully functional Tetris clone using SDL in C++. It features
an endless mode with dynamic speed levels and scoring (more modes might be
added later). It adheres (mostly) to the "Tetris Guideline" specified here:
https://harddrop.com/wiki/Tetris_Guideline
But please note that I am NOT trying to pass this off as an official Tetris
game. This is just a personal project, please don't sue me.

Developer notes:
What's left?
- a README for GitHub

Extra:
- Turn off automatic key repeating (rotate, hard drop, etc)
- Refine debug options - click to create block, pause block drop, respawn block
- Redo menu - don't make it separate from the "game". Not planning on adding more complexity
- Pause menu with resume/restart/return to menu
- Change autorepeat delay/speed to constants
- Sound effects + music
- Customizable controls
- High scores
- Work my way through the Tetris Guideline and make sure everything is implemented.
https://tetris.fandom.com/wiki/Tetris_Guideline

Known bugs:
- Line clear flash appears to have inconsistent speed. Framerate issues?
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
#include "Constants.h"
#include "Menu.h"

//Window and screen surface
SDL_Window* gWindow = NULL;
SDL_Renderer* gRenderer = NULL;

//Game state
int gGameState;

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
	gWindow = SDL_CreateWindow("Turner Vo's Tetris", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN);
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

	//Initialize game
	Menu menu;
	Game game;
	gGameState = MENU;

	//Main loop
	while (!menu.quitting() && !game.quitting()) {
		if (gGameState == MENU) {
			menu.update();
		}
		else if (gGameState == GAME) {
			game.update();
		}
	}

	//End of program
	end();

	return 0;
}