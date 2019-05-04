//Player.h - declaration of the Player class, which handles:
//- processing player movement and rotation
//- rendering the player's blocks on the screen using Playfield::render()
#pragma once
#ifndef PLAYER_H
#define PLAYER_H

#include <SDL.h>
#include <queue>
#include <algorithm>
#include <random>
#include <chrono>
#include "Playfield.h"

//const int NUM_PLAYER_BLOCKS = 4;
const int NUM_DIMENSIONS = 2;
const int NUM_NEXT_PIECES = 6;
const double SOFT_DROP_G = 0.5; //moves down this amount per frame
const int LOCK_SPEED = 40; //in frames
const int MAX_GROUND_ACTIONS = 15;

class Player {
	friend class Playfield;
	friend class UI;
public:
	Player(Playfield* p);
	void startDrop();
	void moveLeft();
	void moveRight();
	void rotateLeft();
	void rotateRight();
	void applySoftDrop(bool);
	void hardDrop();
	void holdBlock();
	void startMoveLeft();
	void startMoveRight();
	void endMoveLeft();
	void endMoveRight();
	void update(); //called once per frame (60 fps)
	void render(); //called once per frame processing loop
	void _debug_cyclePiece(); //manually select piece (debug function)
	void _debug_clearPlayfield();
private:
	void applyGravity();
	void setOrientation(int);
	void setBlockCoords(int, int, int, int, int, int, int, int);
	void spawnBlock();
	bool isColliding(); //checks collision with playfield blocks for rotation
	void addBlocksToQueue();
	void getNextBlock();
	bool isTouchingGround();
	void applyLock();
	void landBlock();
	bool wallKick(int prev);
	void processMovement(); //for autorepeat/DAS
	int getNumLinesCleared(); //called by UI to update displayed text
	void incrementNumLinesCleared(); //called by Playfield to update numLinesCleared

	void renderPlayer();
	void renderNextBlocks();
	void renderHeldBlock();
	void renderGhostBlock();

	Playfield* playfield; //specifies which playfield the player is on, in case there are multiple players
	//These coordinates are relative to the VISIBLE playing field. Too lazy to rework it at this point
	int x, y; // Grid coordinates for center of rotation, except I and O, which in default orientation have the "center" to the left and down
	int type; // type of block. i/j/l/o/s/t/z correspond to 0-6
	int orientation; //rotational state. 0 is default, increments as rotates clockwise up to 3, then loops back to 0
	int blockCoords[NUM_PLAYER_BLOCKS][NUM_DIMENSIONS]; //array of relative grid coords filled by player block. initially stored left->right top->bottom
	int score; //player score earned from clearing lines
	int level; //determines gravity
	double gravity; //corresponding to "G" values on wiki. automatically calculated from level
	double effGravity; //may be more than gravity if soft drop is active
	double gravityProgress; //when above 1, moves player down and decrements by 1
	std::queue<int> blockQueue; //queue of "next blocks"
	int blockBag[NUM_BLOCK_TYPES]; //used as part of randomizer. one of each block
	int lockProgress;
	int heldBlock;
	bool holdUsed;
	bool holdingLeft; //for autorepeat movement
	bool holdingRight;
	int moveProgress; //for autorepeat movement
	int groundActions; //player piece locks after MAX_GROUND_ACTIONS
	int numLinesCleared; //tracks number of lines cleared so far this game
	bool softDropping; //tracks whether soft drop button is pressed

	//wall kick tables, not including test 1
	//still need to check all of these work as expected!
	const int wallKickData_JLSTZ[8][4][2] =
	{ { {-1, 0}, {-1, -1}, {0, 2}, {-1, 2} },
	  { {1, 0}, {1, 1}, {0, -2}, {1, -2} },
	  { {1, 0}, {1, 1}, {0, -2}, {1, -2} },
	  { {-1, 0}, {-1, -1}, {0, 2}, {-1, 2} },
	  { {1, 0}, {1, -1}, {0, 2}, {1, 2} },
	  { {-1, 0}, {-1, 1}, {0, -2}, {-1, -2} },
	  { {-1, 0}, {-1, 1}, {0, -2}, {-1, -2} },
	  { {1, 0}, {1, -1}, {0, 2}, {1, 2} } };

	const int wallKickData_I[8][4][2] =
	{ { {-2, 0}, {1, 0}, {-2, 1}, {1, -2} },
      { {2, 0}, {-1, 0}, {2, -1}, {-1, 2} },
      { {-1, 0}, {2, 0}, {-1, -2}, {2, 1} },
      { {1, 0}, {-2, 0}, {1, 2}, {-2, -1} },
      { {2, 0}, {-1, 0}, {2, -1}, {-1, 2} },
      { {-2, 0}, {1, 0}, {-2, 1}, {1, -2} },
      { {1, 0}, {-2, 0}, {1, 2}, {-2, -1} },
      { {-1, 0}, {2, 0}, {-1, -2}, {2, 1} } };
};

#endif