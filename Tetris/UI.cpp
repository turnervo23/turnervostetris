#include "UI.h"
#include "Playfield.h"
#include "Player.h"

extern SDL_Renderer* gRenderer;

UI::UI(Playfield* p) {
	playfield = p;
	playfield->ui = this;

	timeFont = TTF_OpenFont("./font/consola.ttf", 16);
	timeTextColor = { 0xFF, 0xFF, 0xFF, 0xFF };
	setTime(0);

	scoreFont = TTF_OpenFont("./font/consola.ttf", 16);
	scoreTextColor = { 0xFF, 0xFF, 0xFF, 0xFF };
	setScore(0);

	levelFont = TTF_OpenFont("./font/consola.ttf", 16);
	levelTextColor = { 0xFF, 0xFF, 0xFF, 0xFF };
	setLevel(0);

	linesFont = TTF_OpenFont("./font/consola.ttf", 16);
	linesTextColor = { 0xFF, 0xFF, 0xFF, 0xFF };
	setLines(0);

	clearTypeFont = TTF_OpenFont("./font/consola.ttf", 16);
	clearTypeColor = { 0xd0, 0xd0, 0x00, 0xFF };
	clearTypeTimer = CLEAR_TYPE_DISPLAY_TIME;

	comboFont = TTF_OpenFont("./font/consola.ttf", 16);
	comboTextColor = { 0x00, 0xb0, 0xFF, 0xFF };
	setCombo(0);

	gameOverStr = "GAME OVER";
	gameOverFont = TTF_OpenFont("./font/consolab.ttf", 32);
	gameOverTextColor = { 0xFF, 0xFF, 0xFF, 0xFF };
	gameOverTexture.loadFromText(gameOverStr, gameOverFont, gameOverTextColor);
	gameOverTimer = 0;

	countdownStrs[0] = "Ready?";
	countdownStrs[1] = "Go!";
	countdownFont = TTF_OpenFont("./font/consolab.ttf", 32);
	countdownTextColor = { 0xFF, 0xFF, 0xFF, 0xFF };
	for (int i = 0; i < 2; i++) {
		countdownTextures[i].loadFromText(countdownStrs[i], countdownFont, countdownTextColor);
	}
	countdownStage = 0;
	countdownTimer = 0;

	allClearStr = "All Clear!";
	allClearFont = TTF_OpenFont("./font/consolab.ttf", 32);
	allClearTextColor = { 0xFF, 0xFF, 0xFF, 0xFF };
	allClearTexture.loadFromText(allClearStr, allClearFont, allClearTextColor);
	allClearTimer = ALL_CLEAR_DISPLAY_TIME;

	gameControlsStr = "[Controls]\nMove: Left/Right\nRotate: Z/X\nSoft Drop: Down\nHard Drop: Up\nHold: Shift";
	gameControlsFont = TTF_OpenFont("./font/consola.ttf", 12);
	gameControlsTextColor = { 0xFF, 0xFF, 0xFF, 0xFF };
	gameControlsTexture.loadFromText(gameControlsStr, gameControlsFont, gameControlsTextColor);

	nextPieceStr = "NEXT";
	nextPieceFont = TTF_OpenFont("./font/consola.ttf", 16);
	nextPieceTextColor = { 0xFF, 0xFF, 0xFF, 0xFF };
	nextPieceTexture.loadFromText(nextPieceStr, nextPieceFont, nextPieceTextColor);

	holdStr = "HOLD";
	holdFont = TTF_OpenFont("./font/consola.ttf", 16);
	holdTextColor = { 0xFF, 0xFF, 0xFF, 0xFF };
	holdTexture.loadFromText(holdStr, holdFont, holdTextColor);
}

void UI::render() {
	renderTime();
	renderScore();
	renderLevel();
	renderLines();
	renderGameControls();
	renderNextPieceText();
	renderHoldText();
	if (clearTypeTimer < CLEAR_TYPE_DISPLAY_TIME) {
		renderClearType();
	}
	if (playfield->player->combo >= 1) {
		renderCombo();
	}
	if (gameOverTimer >= GAME_OVER_TEXT_DELAY) {
		renderGameOver();
	}
	if (countdownStage <= 1) {
		renderCountdown();
	}
	if (allClearTimer < ALL_CLEAR_DISPLAY_TIME) {
		renderAllClear();
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
	timeTexture.render(playfield->x - 150, playfield->y + 116);
}

//Update function, called once per frame
void UI::update() {
	if (playfield->countdown) {
		if (countdownTimer < COUNTDOWN_FRAMES_PER_STAGE) { //increment timer
			countdownTimer += 1;
		}
		else { //stage finished, advance to next
			countdownStage++;
			countdownTimer = 0;
		}

		if (countdownStage > 1) { //countdown finished, start gameplay
			playfield->startGame();
		}
	}
	else if (playfield->gameOver) {
		//game over timer
		if (gameOverTimer < GAME_OVER_TEXT_DELAY) {
			gameOverTimer += 1;
		}
	}
	else {
		setTime(SDL_GetTicks() - startTime); //update time unless game is over
		setScore(playfield->player->score);
		setLevel(playfield->player->level);
		setLines(playfield->player->getNumLinesCleared());

		//clear type timer
		if (clearTypeTimer < CLEAR_TYPE_DISPLAY_TIME) {
			clearTypeTimer += 1;
		}

		//all clear timer
		if (allClearTimer < ALL_CLEAR_DISPLAY_TIME) {
			allClearTimer += 1;
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

//Renders the starting countdown text. stages 0-4: READY, 3, 2, 1, GO
void UI::renderCountdown() {
	SDL_RenderSetClipRect(gRenderer, NULL);
	int w, h;
	TTF_SizeText(countdownFont, countdownStrs[countdownStage].c_str(), &w, &h); //get dimensions of countdown texture

	countdownTextures[countdownStage].render(playfield->x + 160 - w/2, playfield->y + 100);
}

//Sets the score. Obtained from this->playfield->player
void UI::setScore(int s) {
	scoreStr = "Score: " + std::to_string(s);
	scoreTexture.loadFromText(scoreStr, scoreFont, scoreTextColor);
}

//Renders the score
void UI::renderScore() {
	SDL_RenderSetClipRect(gRenderer, NULL);
	scoreTexture.render(playfield->x - 150, playfield->y + 148);
}

//Sets the player level. Obtained from this->playfield->player
void UI::setLevel(int l) {
	levelStr = "Level: " + std::to_string(l);
	levelTexture.loadFromText(levelStr, levelFont, levelTextColor);
}

//Renders the player level
void UI::renderLevel() {
	SDL_RenderSetClipRect(gRenderer, NULL);
	levelTexture.render(playfield->x - 150, playfield->y + 180);
}

//Sets the number of lines cleared. Obtained from this->playfield->player
void UI::setLines(int l) {
	linesStr = "Lines: " + std::to_string(l);
	linesTexture.loadFromText(linesStr, linesFont, linesTextColor);
}

//Renders the number of lines cleared
void UI::renderLines() {
	SDL_RenderSetClipRect(gRenderer, NULL);
	linesTexture.render(playfield->x - 150, playfield->y + 212);
}

//Sets the "clear type" (i.e. single, double, tetris, t-spin)
void UI::setClearType(std::string ct) {
	clearTypeStr = ct + "!";
	clearTypeTexture.loadFromText(clearTypeStr, clearTypeFont, clearTypeColor);
	clearTypeTimer = 0;
}

//Renders the clear type (only called after recent clear)
void UI::renderClearType() {
	SDL_RenderSetClipRect(gRenderer, NULL);
	clearTypeTexture.render(playfield->x - 150, playfield->y + 244);
}

//Sets the combo value
void UI::setCombo(int c) {
	comboStr = "Combo: " + std::to_string(playfield->player->combo);
	comboTexture.loadFromText(comboStr, comboFont, comboTextColor);
}

//Renders the combo value
void UI::renderCombo() {
	SDL_RenderSetClipRect(gRenderer, NULL);
	comboTexture.render(playfield->x - 150, playfield->y + 292);
}

//Sets the "All Clear" status
void UI::setAllClear() {
	allClearTimer = 0;
}

//Renders the "All Clear" message
void UI::renderAllClear() {
	SDL_RenderSetClipRect(gRenderer, NULL);
	int w, h;
	TTF_SizeText(allClearFont, allClearStr.c_str(), &w, &h); //get dimensions of all clear texture

	allClearTexture.render(playfield->x + 160 - w / 2, playfield->y + 100);
}

//Renders the game controls in the corner (should be moved to an options menu later)
void UI::renderGameControls() {
	SDL_RenderSetClipRect(gRenderer, NULL);
	gameControlsTexture.render(playfield->x + 350, playfield->y + 500);
}

//Renders the "NEXT" text in next piece box
void UI::renderNextPieceText() {
	SDL_RenderSetClipRect(gRenderer, NULL);
	nextPieceTexture.render(playfield->x + (32 * FIELD_WIDTH) + 16, playfield->y + 8);
}

//Renders the "HOLD" text in held piece box
void UI::renderHoldText() {
	SDL_RenderSetClipRect(gRenderer, NULL);
	holdTexture.render(playfield->x - 136, playfield->y + 8);
}