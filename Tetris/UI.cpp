#include "UI.h"
#include "Playfield.h"
#include "Player.h"

UI::UI(Playfield* p) {
	playfield = p;
	playfield->ui = this;

	timeFont = TTF_OpenFont("./font/consola.ttf", 16);
	timeTextColor = { 0, 0, 0, 0 };

	linesFont = TTF_OpenFont("./font/consola.ttf", 16);
	linesTextColor = { 0, 0, 0, 0 };

	gameOverStr = "GAME OVER";
	gameOverFont = TTF_OpenFont("./font/consolab.ttf", 32);
	gameOverTextColor = { 0, 0, 0, 0 };
	gameOverTexture.loadFromText(gameOverStr, gameOverFont, gameOverTextColor);
	gameOverTimer = 0;
}

void UI::render() {
	renderTime();
	renderLines();
	if (gameOverTimer >= GAME_OVER_TEXT_DELAY) {
		renderGameOver();
	}
}

void UI::setStartTime(int t) {
	startTime = t;
}

void UI::setTime(int t) {
	time = t;

	//Convert to min/sec/ms
	int min = time / 60000;
	int sec = (time - min * 60000) / 1000;
	int ms = (time - min * 60000 - sec * 1000);

	//Format
	std::string minStr = std::to_string(min);
	while (minStr.size() < 2) {
		minStr = "0" + minStr;
	}
	std::string secStr = std::to_string(sec);
	while (secStr.size() < 2) {
		secStr = "0" + secStr;
	}
	std::string msStr = std::to_string(ms);
	while (msStr.size() < 3) {
		msStr = "0" + msStr;
	}

	timeStr = "Time: " + minStr + ":" + secStr + "." + msStr;
	if (min > 99) {
		timeStr = "Time: 99:59.999";
	}

	timeTexture.loadFromText(timeStr, timeFont, timeTextColor);
}

void UI::renderTime() {
	SDL_RenderSetClipRect(gRenderer, NULL);
	timeTexture.render(playfield->x - 150, playfield->y + 100);
}

void UI::update() {
	if (playfield->gameOver == false) {
		setTime(SDL_GetTicks() - startTime); //update time unless game is over
		setLines(playfield->player->getNumLinesCleared());
	}
	else { //game over, start timer
		if (gameOverTimer < GAME_OVER_TEXT_DELAY) {
			gameOverTimer += 1;
		}
	}
}

//Renders the GAME OVER message in the middle of the playfield after it's been one second since game over
void UI::renderGameOver() {
	SDL_RenderSetClipRect(gRenderer, NULL);
	int w, h;
	TTF_SizeText(gameOverFont, gameOverStr.c_str(), &w, &h); //get dimensions of game over texture

	gameOverTexture.render(playfield->x + 160 - w/2, playfield->y + 100);
}

//Sets the number of lines cleared. Obtained from this->playfield->player
void UI::setLines(int l) {
	linesStr = "Lines: " + std::to_string(l);
	linesTexture.loadFromText(linesStr, linesFont, linesTextColor);
}

//Renders the number of lines cleared
void UI::renderLines() {
	SDL_RenderSetClipRect(gRenderer, NULL);
	linesTexture.render(playfield->x - 150, playfield->y + 150);
}