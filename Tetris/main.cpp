/*
Xtvo's Tetris:
My goal is to make a fully functional Tetris clone using SDL in C++.
The first finished version should have at least:
- Main menu
- Endless mode (aka Marathon) with dynamic speed levels and scoring.
Then I can start adding more stuff if I want.

What's next on the agenda?
- Add scoring for T-spin mini, B2B
T-spin mini notes - from http://kitaru.1101b.com/tc/mini.html:
"a t-spin is mini if there is an open corner on the point-end,
it clears one line or less, or reached its final (x,y) position by way of the t-spin triple wall kick"
I think the last one is the opposite - DIDN'T reach its final position with TST twist.
- Ready 3 2 1 go
- Main menu
- Sound effects + music
- Customizable controls
- Turn off automatic key repeating
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

	Playfield playfield1;
	playfield1.loadTextures();

	Player player1(&playfield1);
	player1.startDrop();

	UI ui1(&playfield1);
	int startTime = SDL_GetTicks(); //frame counting starts here
	ui1.setStartTime(startTime);

	//Main loop
	bool quit = false;
	SDL_Event e;
	const Uint8* keystate = SDL_GetKeyboardState(NULL);
	gCurFrame = -1;
	gPrevFrame = -1;
	while (!quit) {
		//TODO: a solution for SDL_GetTicks unsigned overflow. it's ~49 days though so kinda low priority
		gCurFrame = int (floor((SDL_GetTicks() - startTime) * 60 / 1000.0));
		for (int i = gPrevFrame; i < gCurFrame; i++) { //process new frames
			//Handle events on queue
			while (SDL_PollEvent(&e) != 0) {
				//Quit program
				if (e.type == SDL_QUIT)
					quit = true;

				//Pressed keys
				else if (e.type == SDL_KEYDOWN) {
					switch (e.key.keysym.sym) {
					case SDLK_LEFT:
						player1.startMoveLeft();
						break;
					case SDLK_RIGHT:
						player1.startMoveRight();
						break;
					case SDLK_DOWN:
						player1.applySoftDrop(true);
						break;
					case SDLK_UP:
						player1.hardDrop();
						break;
					case SDLK_z:
						player1.rotateLeft();
						break;
					case SDLK_x:
						player1.rotateRight();
						break;
					case SDLK_g: //temporary to test game over
						playfield1.endGame();
						break;
					case SDLK_LSHIFT:
						player1.holdBlock();
						break;
					//debug only, remove in finished version
					case SDLK_c:
						player1._debug_cyclePiece();
						break;
					case SDLK_v:
						player1.startDrop();
						break;
					case SDLK_b:
						player1._debug_clearPlayfield();
						break;
					}
				}

				//Released keys
				else if (e.type == SDL_KEYUP) {
					switch (e.key.keysym.sym) {
					case SDLK_LEFT:
						player1.endMoveLeft();
						break;
					case SDLK_RIGHT:
						player1.endMoveRight();
						break;
					case SDLK_DOWN:
						player1.applySoftDrop(false);
						break;
					}
				}
			}

			//Update objects
			//playfield1.update();
			player1.update();
			ui1.update();
		}

		//Clear screen
		SDL_RenderClear(gRenderer);

		//Render objects
		playfield1.render();
		player1.render();
		ui1.render();

		//Update screen
		SDL_SetRenderDrawColor(gRenderer, 0xFF, 0xFF, 0xFF, 0xFF);
		SDL_RenderPresent(gRenderer);

		gPrevFrame = gCurFrame; //update frame count
	}

	//End of program
	end();

	return 0;
}