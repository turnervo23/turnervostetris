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
#include "Constants.h"
#include "Texture.h"

class Game;
class Player;
class UI;
class Texture;

class Playfield {
	friend class Player;
	friend class UI;
public:
	Playfield(Game*);
	void loadTextures();
	void setTextureColorByBlockType(Texture &t, int blockType);
	void update(); //called once per frame (60 fps)
	void render(); //called once per frame processing loop
	void endGame(); //only public for debug
private:
	void renderPlayfield();
	void renderBlocks();
	void checkLineClear();
	void renderLineClear();
	void renderLockFlash();
	
	Game *game;
	Player *player;
	UI *ui;

	Texture textures[NUM_BLOCK_TYPES];
	Texture lineFlashTexture;
	Texture lockFlashTexture;
	Texture playerGlowTexture;
	//These coordinates are 0-9/0-39, top down, left to right . y=20-39 is visible playing field.
	int grid[FIELD_HEIGHT][FIELD_WIDTH]; // holds ids of blocks on grid, not including player?. -1 is empty
	int x, y; //playfield's location on screen. top-left of highest fully visible row
	
	bool suspended; //if waiting for line clear animation to finish. player doesn't update if this is true

	bool linesClearing[FIELD_HEIGHT]; //true if line with given index is being cleared, false otherwise.
	bool lineClearing; //true if ANY line is being cleared
	int lineClearStartFrame;
	int lineClearCurFrame;
	bool backToBack; //tracks whether the previous clear (or T-Spin) continues back-to-back chain

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