//Playfield.h - declaration of the Playfield class, which handles:
//- loading and rendering the blocks for the player, ones that have landed, and hold/next pieces
//- keeping track of which grid spaces have which blocks in them
#pragma once
#include <SDL.h>
#include <SDL_image.h>
#include <string>
#include <iostream>
#include "Texture.h"

const int NUM_BLOCK_TYPES = 7;
const int FIELD_WIDTH = 10;
const int FIELD_HEIGHT = 40;
const int VISIBLE_FIELD_HEIGHT = 20;
const int NO_BLOCK = -1;

class Playfield {
	friend class Player;
public:
	Playfield();
	void loadTextures();
	void update(); //called once per frame
private:
	void render();
	void renderBlocks();
	void clearLines();

	Texture textures[NUM_BLOCK_TYPES];
	int grid[FIELD_HEIGHT][FIELD_WIDTH]; // holds ids of blocks on grid, not including player?. -1 is empty
	int x, y; //playfield's location on screen. top-left of highest fully visible row.
	SDL_Rect playfieldClip; //clipRect for playfield
	SDL_Rect nextBlockClip; //clipRect for next piece display
	SDL_Rect heldBlockClip;
};