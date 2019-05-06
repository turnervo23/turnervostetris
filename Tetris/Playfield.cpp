#include "Playfield.h"
#include "UI.h"
#include "Player.h"

Playfield::Playfield() {
	//Empty playfield grid
	for (int r = 0; r < FIELD_HEIGHT; r++) {
		for (int c = 0; c < FIELD_WIDTH; c++) {
			grid[r][c] = NO_BLOCK;
		}
	}

	//Sets playfield location. top left of row 20
	x = 176;
	y = 40;

	playfieldClip.x = x - 8;
	playfieldClip.y = y - 8;
	playfieldClip.w = (32 * FIELD_WIDTH) + 16;
	playfieldClip.h = (32 * VISIBLE_FIELD_HEIGHT) + 16;

	nextBlockClip.x = x + (32 * FIELD_WIDTH) + 8;
	nextBlockClip.y = y - 8;
	nextBlockClip.w = 160;
	nextBlockClip.h = 336;

	heldBlockClip.x = x - 8 - 160;
	heldBlockClip.y = y - 8;
	heldBlockClip.w = 160;
	heldBlockClip.h = 72;

	suspended = false;
	gameOver = false;

	lineClearing = false;
	for (int r = 0; r < FIELD_HEIGHT; r++) {
		linesClearing[r] = false;
	}

	player = NULL;
	ui = NULL;
}

//Loads the textures for the blocks
void Playfield::loadTextures() {
	std::string path;
	for (int i = 0; i < NUM_BLOCK_TYPES; i++) {
		//Pathname for block images. Subject to change
		//path = "./img/" + std::to_string(i) + ".png";
		path = "./img/block.png";
		textures[i].loadFromFile(path);

		//Colors each block texture separately
		int r, g, b;
		switch (i) {
		case I_BLOCK:
			r = 0;
			g = 255;
			b = 255;
			break;
		case J_BLOCK:
			r = 0;
			g = 0;
			b = 255;
			break;
		case L_BLOCK:
			r = 255;
			g = 127;
			b = 0;
			break;
		case O_BLOCK:
			r = 255;
			g = 255;
			b = 0;
			break;
		case S_BLOCK:
			r = 0;
			g = 255;
			b = 0;
			break;
		case T_BLOCK:
			r = 255;
			g = 0;
			b = 255;
			break;
		case Z_BLOCK:
			r = 255;
			g = 0;
			b = 0;
			break;
		}

		textures[i].setColor(r, g, b);
	}

	//Line clear flash texture
	path = "./img/lineflash.png";
	lineFlashTexture.loadFromFile(path);

	//Lock clear flash texture
	path = "./img/lockflash.png";
	lockFlashTexture.loadFromFile(path);
	lockFlashTexture.setAlpha(160);

	//Player glow texture
	path = "./img/lockflash.png";
	playerGlowTexture.loadFromFile(path);
	playerGlowTexture.setAlpha(48);
}

//Calls all rendering functions
void Playfield::render() {
	renderPlayfield();
	renderBlocks();
	if (lineClearing == true) {
		renderLineClear();
	}
	if (lockFlashing == true) {
		renderLockFlash();
	}
}

void Playfield::renderPlayfield() {
	SDL_RenderSetClipRect(gRenderer, &playfieldClip);
	//Render gridlines (2 px thickness for symmetry)
	//Horizontal
	SDL_SetRenderDrawColor(gRenderer, 0xb0, 0xb0, 0xb0, 0xFF); //light gray
	for (int i = 0; i <= FIELD_HEIGHT; i++) {
		SDL_RenderDrawLine(gRenderer, x, y + (32 * i), x + (32 * FIELD_WIDTH), y + (32 * i));
		SDL_RenderDrawLine(gRenderer, x, y + (32 * i) - 1, x + (32 * FIELD_WIDTH), y + (32 * i) - 1);
	}
	//Vertical
	for (int j = 0; j <= FIELD_WIDTH; j++) {
		SDL_RenderDrawLine(gRenderer, x + (32 * j), y, x + (32 * j), y + (32 * VISIBLE_FIELD_HEIGHT));
		SDL_RenderDrawLine(gRenderer, x + (32 * j) - 1, y, x + (32 * j) - 1, y + (32 * VISIBLE_FIELD_HEIGHT));
	}

	//8 px border
	SDL_SetRenderDrawColor(gRenderer, 0x40, 0x40, 0x40, 0xFF); //dark gray
	for (int i = 0; i < 8; i++) {
		SDL_RenderDrawLine(gRenderer, x - 9, y - 9 + i,
			x + (32 * FIELD_WIDTH) + 8, y - 9 + i); //top
		SDL_RenderDrawLine(gRenderer, x - 9, y + (32 * VISIBLE_FIELD_HEIGHT) + 8 - i,
			x + (32 * FIELD_WIDTH) + 8, y + (32 * VISIBLE_FIELD_HEIGHT) + 8 - i); //bottom
		SDL_RenderDrawLine(gRenderer, x - 9 + i, y - 9,
			x - 9 + i, y + (32 * VISIBLE_FIELD_HEIGHT) + 8); // left
		SDL_RenderDrawLine(gRenderer, x + (32 * FIELD_WIDTH) + 8 - i, y - 9,
			x + (32 * FIELD_WIDTH) + 8 - i, y + (32 * VISIBLE_FIELD_HEIGHT) + 8); //right
	}
}

void Playfield::renderBlocks() {
	//Render blocks
	for (int r = -1; r < VISIBLE_FIELD_HEIGHT; r++) {
		for (int c = 0; c < FIELD_WIDTH; c++) {
			if (grid[r + 20][c] != NO_BLOCK) {
				textures[grid[r + 20][c]].render(x + (32 * c), y + (32 * r));
			}
		}
	}
}

//Update function for playfield; called every frame
void Playfield::update() {
	/*
	if (lineClearing == false) {
		checkLineClear();
	}
	*/
}

//Clear any lines made. No scoring implemented yet
void Playfield::checkLineClear() {
	bool line;
	int numLinesClearedAtOnce = 0; //tracks the number of lines cleared at once for scoring
	for (int r = 0; r < FIELD_HEIGHT; r++) { //for each row, check if line
		line = true;
		for (int c = 0; c < FIELD_WIDTH; c++) { //if any in row NO_BLOCK, no line
			if (grid[r][c] == NO_BLOCK) {
				line = false;
			}
		}
		if (line == true) { //if line, start line clear animation
			numLinesClearedAtOnce += 1;
			lineClearing = true;
			linesClearing[r] = true;
			suspended = true;
			lineClearStartFrame = gCurFrame;
			player->incrementNumLinesCleared();
		}
	}

	//Tally player score for lines cleared
	if (numLinesClearedAtOnce == 1) { //single
		player->score += 100 * player->level;
		ui->setClearType("Single");
	}
	else if (numLinesClearedAtOnce == 2) { //double
		player->score += 200 * player->level;
		ui->setClearType("Double");
	}
	else if (numLinesClearedAtOnce == 3) { //triple
		player->score += 300 * player->level;
		ui->setClearType("Triple");
	}
	else if (numLinesClearedAtOnce == 4) { //tetris
		player->score += 500 * player->level;
		ui->setClearType("Tetris");
	}
}

//Line clear animation. Flashes on and off. Suspends gameplay for LINE_CLEAR_DELAY frames
void Playfield::renderLineClear() {
	lineClearCurFrame = gCurFrame;

	if (lineClearCurFrame - lineClearStartFrame >= LINE_CLEAR_DELAY) { //animation done
		for (int r = 0; r < FIELD_HEIGHT; r++) { //top down. ensures cleared lines aren't moved
			if (linesClearing[r] == true) {
				for (int c = 0; c < FIELD_WIDTH; c++) { //remove line
					grid[r][c] = NO_BLOCK;
				}

				for (int rowsAbove = r - 1; rowsAbove > -1; rowsAbove--) { //move rows above down
					for (int c = 0; c < FIELD_WIDTH; c++) {
						grid[rowsAbove + 1][c] = grid[rowsAbove][c];
					}
				}

				linesClearing[r] = false;
			}
		}
		lineClearing = false;
		suspended = false;
	}
	else if ((lineClearCurFrame - lineClearStartFrame) % LINE_CLEAR_FLASH_FREQ == 0) { //render flash every 4 frames
		for (int r = 0; r < FIELD_HEIGHT; r++) {
			if (linesClearing[r] == true) {
				SDL_RenderSetClipRect(gRenderer, &playfieldClip);
				lineFlashTexture.render(x, y + (r - 20) * 32);
			}
		}	
	}
}

//Ends the game, suspending gameplay indefinitely (later: until menu selection by player)
void Playfield::endGame() {
	suspended = true;
	gameOver = true;
}

//Render the lock flash texture at the block locations of the recently landed piece
void Playfield::renderLockFlash() {
	lockFlashCurFrame = gCurFrame;

	if (lockFlashCurFrame - lockFlashStartFrame >= LOCK_FLASH_TIME) { //animation done
		lockFlashing = false;
	}
	else { //render
		SDL_RenderSetClipRect(gRenderer, &playfieldClip);
		for (int i = 0; i < NUM_PLAYER_BLOCKS; i++) {
			lockFlashTexture.render(x + 32*lockFlashX[i], y + 32*lockFlashY[i]);
		}
	}
}