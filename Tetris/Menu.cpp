#include "Menu.h"

extern SDL_Renderer* gRenderer;

Menu::Menu() {
	menuOptionStrs[0] = "Play";
	menuOptionStrs[1] = "Quit";
	menuOptionFont = TTF_OpenFont("./font/consola.ttf", 32);
	menuOptionTextColor = { 0, 0, 0, 0 };
	for (int i = 0; i < NUM_MENU_OPTIONS; i++) {
		menuOptionTextures[i].loadFromText(menuOptionStrs[i], menuOptionFont, menuOptionTextColor);
	}

	titleStr = "Xtvo's Tetris";
	titleFont = TTF_OpenFont("./font/consolab.ttf", 64);
	titleTextColor = { 0, 0, 0, 0 };
	titleTexture.loadFromText(titleStr, titleFont, titleTextColor);

	cursorTexture.loadFromFile("./img/menuCursor.png");

	highlightedOption = PLAY;
	quit = false;
}

void Menu::update() {
	while (SDL_PollEvent(&event) != 0) {
		//Quit program
		if (event.type == SDL_QUIT)
			quit = true;

		//Pressed keys
		else if (event.type == SDL_KEYDOWN) {
			switch (event.key.keysym.sym) {
			case SDLK_UP:
				highlightedOption--;
				if (highlightedOption < 0) { //if at top, go to bottom
					highlightedOption = NUM_MENU_OPTIONS - 1;
				}
				break;
			case SDLK_DOWN:
				highlightedOption++;
				if (highlightedOption > NUM_MENU_OPTIONS - 1) { //if at bottom, go to top
					highlightedOption = 0;
				}
				break;
			}
		}
	}

	//Clear screen
	SDL_RenderClear(gRenderer);

	//Render objects
	render();

	//Update screen
	SDL_SetRenderDrawColor(gRenderer, 0xFF, 0xFF, 0xFF, 0xFF);
	SDL_RenderPresent(gRenderer);
}

void Menu::render() {
	renderMenuOptions();
	renderTitle();
	renderCursor();
}

void Menu::renderMenuOptions() {
	SDL_RenderSetClipRect(gRenderer, NULL);
	for (int i = 0; i < NUM_MENU_OPTIONS; i++) {
		menuOptionTextures[i].render(100, 200 + 50*i);
	}
}

void Menu::renderTitle() {
	SDL_RenderSetClipRect(gRenderer, NULL);
	titleTexture.render(100, 100);
}

void Menu::renderCursor() {
	SDL_RenderSetClipRect(gRenderer, NULL);
	cursorTexture.render(60, 200 + 50*highlightedOption); //renders to the left of highlighted menu option
}

//Returns true if user has quit the game
bool Menu::quitting() {
	return quit;
}