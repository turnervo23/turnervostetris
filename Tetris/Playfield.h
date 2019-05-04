//Playfield.h - declaration of the Playfield class, which handles:
//- loading and rendering the blocks for the player, ones that have landed, and hold/next pieces
//- keeping track of which grid spaces have which blocks in them
#pragma once
#ifndef PLAYFIELD_H
#define PLAYFIELD_H

#include <SDL.h>
#include <SDL_image.h>
#include <string>
#include <iostream>
#include "Texture.h"

class Player;
class UI;

extern int gCurFrame;

const int NUM_PLAYER_BLOCKS = 4;
const int NUM_BLOCK_TYPES = 7;
const int FIELD_WIDTH = 10;
const int FIELD_HEIGHT = 40;
const int VISIBLE_FIELD_HEIGHT = 20;
const int LINE_CLEAR_DELAY = 30; //in frames
const int LOCK_FLASH_TIME = 10; //in frames

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
	void endGame(); //only public for debug
private:
	void renderPlayfield();
	void renderBlocks();
	void checkLineClear();
	void renderLineClear();
	void renderLockFlash();

	Player *player;
	UI *ui;

	Texture textures[NUM_BLOCK_TYPES];
	Texture lineFlashTexture;
	Texture lockFlashTexture;
	//These coordinates are 0-9/0-39, top down, left to right . y=20-39 is visible playing field.
	int grid[FIELD_HEIGHT][FIELD_WIDTH]; // holds ids of blocks on grid, not including player?. -1 is empty
	int x, y; //playfield's location on screen. top-left of highest fully visible row
	
	bool suspended; //if waiting for line clear animation to finish. player doesn't update if this is true

	bool linesClearing[FIELD_HEIGHT]; //true if line with given index is being cleared, false otherwise.
	bool lineClearing; //true if ANY line is being cleared
	int lineClearStartFrame;
	int lineClearCurFrame;

	bool lockFlashing; //whether "lock flash" is occurring (animation for piece lock)
	int lockFlashX[NUM_PLAYER_BLOCKS];
	int lockFlashY[NUM_PLAYER_BLOCKS]; //locations of lock flash using player x/y coords
	int lockFlashStartFrame;
	int lockFlashCurFrame;

	bool gameOver;

	SDL_Rect playfieldClip; //clipRect for playfield
	SDL_Rect nextBlockClip; //clipRect for next piece display
	SDL_Rect heldBlockClip;
};

#endif