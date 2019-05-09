#include "Game.h"

//Constructor
Game::Game() {
	playfield1 = new Playfield(this);
	playfield1->loadTextures();

	player1 = new Player(playfield1);
	player1->startDrop();

	ui1 = new UI(playfield1);
	startTime = SDL_GetTicks(); //frame counting starts here
	ui1->setStartTime(startTime);

	quit = false;
	curFrame = -1;
	prevFrame = -1;
}

//Destructor
Game::~Game() {
	delete playfield1;
	delete player1;
	delete ui1;
}

//Update function, called once per frame
void Game::update() {
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
		//playfield1->update();
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
	SDL_SetRenderDrawColor(gRenderer, 0xFF, 0xFF, 0xFF, 0xFF);
	SDL_RenderPresent(gRenderer);

	prevFrame = curFrame; //update frame count
}

//Returns true if user has quit the game
bool Game::quitting() {
	return quit;
}

//Returns the current frame
int Game::getCurFrame() {
	return curFrame;
}