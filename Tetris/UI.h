#pragma once
#ifndef UI_H
#define UI_H

#include <SDL.h>
#include <SDL_ttf.h>
#include <string>
#include "Constants.h"
#include "Texture.h"

class Playfield;
class Player;
class Texture;

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
	void setCombo(int c);
	void setAllClear();
	void update();
	void render();
private:
	void renderTime();
	void renderScore();
	void renderLevel();
	void renderLines();
	void renderClearType();
	void renderCombo();
	void renderGameOver();
	void renderCountdown();
	void renderAllClear();
	void renderGameControls();

	Playfield* playfield;

	int startTime; //SDL ticks at start
	int time; //SDL ticks

	//Should really put this stuff in a class called UIText or something
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

	Texture comboTexture;
	std::string comboStr;
	TTF_Font* comboFont;
	SDL_Color comboTextColor;
	
	Texture gameOverTexture;
	std::string gameOverStr;
	TTF_Font* gameOverFont;
	SDL_Color gameOverTextColor;
	int gameOverTimer; //displays text after GAME_OVER_TEXT_DELAY

	Texture countdownTextures[2];
	std::string countdownStrs[2];
	TTF_Font* countdownFont;
	SDL_Color countdownTextColor;
	int countdownStage;
	int countdownTimer; //displays text after COUNTDOWN_FRAMES_PER_STAGE

	Texture allClearTexture;
	std::string allClearStr;
	TTF_Font* allClearFont;
	SDL_Color allClearTextColor;
	int allClearTimer;

	Texture gameControlsTexture;
	std::string gameControlsStr;
	TTF_Font* gameControlsFont;
	SDL_Color gameControlsTextColor;
};

#endif