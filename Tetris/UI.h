#pragma once
#ifndef UI_H
#define UI_H

#include <SDL.h>
#include <SDL_ttf.h>
#include <string>
#include "Texture.h"

const int GAME_OVER_TEXT_DELAY = 60;
const int CLEAR_TYPE_DISPLAY_TIME = 120;

class UI {
	friend class Playfield;
	friend class Player;
public:
	UI(Playfield *p);
	void setStartTime(int t);
	void setTime(int t);
	void setScore(int s);
	void setLevel(int l);
	void setLines(int l);
	void setClearType(std::string ct);
	void update();
	void render();
private:
	void renderTime();
	void renderScore();
	void renderLevel();
	void renderLines();
	void renderClearType();
	void renderGameOver();

	Playfield* playfield;

	int startTime; //SDL ticks at start
	int time; //SDL ticks
	Texture timeTexture;
	std::string timeStr; //formatted min:sec:ms
	TTF_Font* timeFont;
	SDL_Color timeTextColor;

	Texture scoreTexture;
	std::string scoreStr;
	TTF_Font* scoreFont;
	SDL_Color scoreTextColor;

	Texture levelTexture;
	std::string levelStr;
	TTF_Font* levelFont;
	SDL_Color levelTextColor;

	Texture linesTexture;
	std::string linesStr;
	TTF_Font* linesFont;
	SDL_Color linesTextColor;

	Texture clearTypeTexture;
	std::string clearTypeStr;
	TTF_Font* clearTypeFont;
	SDL_Color clearTypeColor;
	int clearTypeTimer; //displays text for the duration of the timer
	
	Texture gameOverTexture;
	std::string gameOverStr;
	TTF_Font* gameOverFont;
	SDL_Color gameOverTextColor;
	int gameOverTimer; //displays text after GAME_OVER_TEXT_DELAY
};

#endif