#pragma once
#include <SDL.h>
#include <SDL_ttf.h>
#include "Texture.h"

class UI {
public:
	UI();
	void setTime(int t);
	void setFont(TTF_Font* f);
	void setFontColor(SDL_Color c);
	void update();
	void render();
private:
	int time;
	Texture timeTexture;
	TTF_Font* font;
	SDL_Color textColor;
};