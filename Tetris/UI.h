#pragma once
#include <SDL.h>
#include <SDL_ttf.h>
#include <string>
#include "Texture.h"
#include "Playfield.h"

class UI {
public:
	UI(Playfield *p);
	void setStartTime(int t);
	void setTime(int t);
	void setFont(TTF_Font* f, SDL_Color c);
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
	TTF_Font* font;
	SDL_Color textColor;
	Texture gameOverTexture;
};