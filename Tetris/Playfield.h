//Playfield.h - declaration of the Playfield class, which handles:
//- loading and rendering the blocks for the player, ones that have landed, and hold/next pieces
//- keeping track of which grid spaces have which blocks in them
#pragma once
#include <SDL.h>
#include <SDL_image.h>
#include <string>
#include <iostream>
#include "Texture.h"

extern int gCurFrame;

const int NUM_BLOCK_TYPES = 7;
const int FIELD_WIDTH = 10;
const int FIELD_HEIGHT = 40;
const int VISIBLE_FIELD_HEIGHT = 20;
const int LINE_CLEAR_DELAY = 30; //in frames

const int NO_BLOCK = -1;
enum BlockTypes {
	I_BLOCK,
	J_BLOCK,
	L_BLOCK,
	O_BLOCK,
	S_BLOCK,
	T_BLOCK,
	Z_BLOCK,
};

class Playfield {
	friend class Player;
	friend class UI;
public:
	Playfield();
	void loadTextures();
	void update(); //called once per frame (60 fps)
	void render(); //called once per frame processing loop
private:
	void renderPlayfield();
	void renderBlocks();
	void checkLineClear();
	void renderLineClear();

	Texture textures[NUM_BLOCK_TYPES];
	Texture flashTexture;
	int grid[FIELD_HEIGHT][FIELD_WIDTH]; // holds ids of blocks on grid, not including player?. -1 is empty
	int x, y; //playfield's location on screen. top-left of highest fully visible row.
	bool clearing; //if waiting for line clear animation to finish. player doesn't update if this is true
	bool linesClearing[FIELD_HEIGHT]; //true if line is being cleared, false otherwise.
	int lineClearStartFrame;
	int lineClearCurFrame;
	SDL_Rect playfieldClip; //clipRect for playfield
	SDL_Rect nextBlockClip; //clipRect for next piece display
	SDL_Rect heldBlockClip;
};