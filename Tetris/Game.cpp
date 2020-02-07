#include "Game.h"
#include "Playfield.h"
#include "Player.h"
#include "UI.h"

extern SDL_Renderer* gRenderer;
extern int gGameState;

//Constructor
Game::Game() {
	init();
}

//Destructor
Game::~Game() {
	end();
}

//Initialization
void Game::init() {
	playfield1 = new Playfield(this);
	player1 = new Player(playfield1);
	ui1 = new UI(playfield1);

	startTime = -1; //don't set true start time until game starts updating

	quit = false;
	curFrame = -1;
	prevFrame = -1;
}

//End of game (frees up memory)
void Game::end() {
	delete playfield1;
	delete player1;
	delete ui1;
}

//Update function, called once per frame
void Game::update() {
	if (startTime == -1) { //frame counting starts here
		startTime = SDL_GetTicks();
		ui1->setStartTime(startTime);
	}

	curFrame = int(floor((SDL_GetTicks() - startTime) * 60 / 1000.0));

	for (int i = prevFrame; i < curFrame; i++) { //process new frames
		//Handle events on queue
		while (SDL_PollEvent(&event) != 0) {
			//Quit program
			if (event.type == SDL_QUIT)
				quit = true;

			//Pressed keys
			else if (event.type == SDL_KEYDOWN) {
				switch (event.key.keysym.sym) {
				case SDLK_LEFT:
					player1->startMoveLeft();
					break;
				case SDLK_RIGHT:
					player1->startMoveRight();
					break;
				case SDLK_DOWN:
					player1->applySoftDrop(true);
					break;
				case SDLK_UP:
					player1->hardDrop();
					break;
				case SDLK_z:
					player1->rotateLeft();
					break;
				case SDLK_x:
					player1->rotateRight();
					break;
				case SDLK_LSHIFT:
					player1->holdBlock();
					break;
				//debug only, remove in finished version
				case SDLK_c:
					player1->_debug_cyclePiece();
					break;
				case SDLK_v:
					player1->startDrop();
					break;
				case SDLK_b:
					player1->_debug_clearPlayfield();
					break;
				}
			}

			//Released keys
			else if (event.type == SDL_KEYUP) {
				switch (event.key.keysym.sym) {
				case SDLK_LEFT:
					player1->endMoveLeft();
					break;
				case SDLK_RIGHT:
					player1->endMoveRight();
					break;
				case SDLK_DOWN:
					player1->applySoftDrop(false);
					break;
				}
			}
		}

		//Update objects
		playfield1->update();
		player1->update();
		ui1->update();
	}

	//Clear screen
	SDL_RenderClear(gRenderer);

	//Render objects
	playfield1->render();
	player1->render();
	ui1->render();

	//Update screen
	SDL_SetRenderDrawColor(gRenderer, 0x20, 0x20, 0x20, 0xFF);
	SDL_RenderPresent(gRenderer);

	prevFrame = curFrame; //update frame count

	//Return to menu after GAME_OVER_MENU_DELAY
	if (playfield1->returnToMenu()) {
		gGameState = MENU;
		end();
		init(); //starts a new game preemptively
	}
}

//Returns true if user has quit the game
bool Game::quitting() {
	return quit;
}

//Returns the current frame
int Game::getCurFrame() {
	return curFrame;
}