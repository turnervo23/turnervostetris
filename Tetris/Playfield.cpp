#include "Playfield.h"

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

	heldBlockClip.x = x - 8 - 136;
	heldBlockClip.y = y - 8;
	heldBlockClip.w = 160;
	heldBlockClip.h = 72;
}

//Loads the textures for the blocks
void Playfield::loadTextures() {
	std::string path;
	for (int i = 0; i < NUM_BLOCK_TYPES; i++) {
		//Pathname for block images. Subject to change
		path = "./img/" + std::to_string(i) + ".png";
		textures[i].loadFromFile(path);
	}
}

//Render blocks and gridlines
void Playfield::render() {
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
				textures[grid[r + 20][c]].render(x + (32 * c), y + (32 * r)); //i, j reversed?
			}
		}
	}
}

void Playfield::update() {
	clearLines();
	render();
	renderBlocks();
}

//Clear any lines made. No scoring implemented yet
void Playfield::clearLines() {
	bool line;
	for (int r = 0; r < FIELD_HEIGHT; r++) { //for each row, check if line
		line = true;
		for (int c = 0; c < FIELD_WIDTH; c++) { //if any in row NO_BLOCK, no line
			if (grid[r][c] == NO_BLOCK) {
				line = false;
			}
		}
		if (line == true) {
			for (int c = 0; c < FIELD_WIDTH; c++) { //clear line
				grid[r][c] = NO_BLOCK;
			}
			for (int rowsAbove = r - 1; rowsAbove > -1; rowsAbove--) { //move rows above down
				for (int c = 0; c < FIELD_WIDTH; c++) {
					grid[rowsAbove + 1][c] = grid[rowsAbove][c];
				}
			}
		}
	}
}