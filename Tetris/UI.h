#pragma once
#include <SDL.h>
#include <SDL_ttf.h>
#include <string>
#include "Texture.h"
#include "Playfield.h"

const int GAME_OVER_TEXT_DELAY = 60;

class UI {
public:
	UI(Playfield *p);
	void setStartTime(int t);
	void setTime(int t);
	void update();
	void render();
private:
	void renderTime();
	void renderGameOver();

	Playfield* playfield;

	int startTime; //SDL ticks at start
	int time; //SDL ticks
	std::string timeStr; //formatted min:sec:ms
	Texture timeTexture;
	TTF_Font* timeFont;
	SDL_Color timeTextColor;
	
	Texture gameOverTexture;
	TTF_Font* gameOverFont;
	SDL_Color gameOverTextColor;
	int gameOverTimer; //displays text after GAME_OVER_TEXT_DELAY
};