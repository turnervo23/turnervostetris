#pragma once
#ifndef MENU_H
#define MENU_H

#include <string>
#include <SDL.h>
#include <SDL_ttf.h>
#include "Texture.h"

const int NUM_MENU_OPTIONS = 2;

enum MenuOptions {
	PLAY,
	QUIT
};

class Menu {
public:
	Menu();
	void update();
	void render();
	bool quitting();
private:
	void renderMenuOptions();
	void renderTitle();
	void renderCursor();

	Texture menuOptionTextures[2];
	std::string menuOptionStrs[2];
	TTF_Font* menuOptionFont;
	SDL_Color menuOptionTextColor;

	Texture titleTexture;
	std::string titleStr;
	TTF_Font* titleFont;
	SDL_Color titleTextColor;

	Texture cursorTexture;

	int highlightedOption;
	SDL_Event event; //player input
	bool quit; //whether the player has quit the game or not
};

#endif