//Player.h - declaration of the Player class, which handles:
//- processing player movement and rotation
//- rendering the player's blocks on the screen using Playfield::render()

#pragma once
#include <SDL.h>
#include <queue>
#include <algorithm>
#include <random>
#include <chrono>
#include "Playfield.h"

const int NUM_PLAYER_BLOCKS = 4;
const int NUM_DIMENSIONS = 2;
const int NUM_NEXT_PIECES = 6;
const double SOFT_DROP_G = 1;
const int LOCK_SPEED = 40; //in frames
enum BlockTypes {
	I_BLOCK,
	J_BLOCK,
	L_BLOCK,
	O_BLOCK,
	S_BLOCK,
	T_BLOCK,
	Z_BLOCK,
};

class Player {
public:
	Player(Playfield* p);
	void startDrop();
	void move(SDL_Keycode);
	void rotate(SDL_Keycode);
	void applySoftDrop(bool);
	void hardDrop();
	void holdBlock();
	void update(); //called once per frame
	void _debug_cyclePiece(); //manually select piece (debug function)
	void _debug_clearPlayfield();
private:
	void render();
	void applyGravity();
	void setOrientation(int);
	void setBlockCoords(int, int, int, int, int, int, int, int);
	void spawnBlock();
	bool isColliding(); //checks collision with playfield blocks for rotation
	void addBlocksToQueue();
	void getNextBlock();
	void renderNextBlocks();
	void renderHeldBlock();
	bool isTouchingGround();
	void applyLock();
	void landBlock();
	bool wallKick(int prev);

	Playfield* playfield; //specifies which playfield the player is on, in case there are multiple players
	int x, y; // Grid coordinates for center of rotation, except I and O, which in default orientation have the "center" to the left and down
	int type; // type of block. i/j/l/o/s/t/z correspond to 0-6
	int orientation; //rotational state. 0 is default, increments as rotates clockwise up to 3, then loops back to 0
	int blockCoords[NUM_PLAYER_BLOCKS][NUM_DIMENSIONS]; //array of relative grid coords filled by player block. initially stored left->right top->bottom
	int level; //determines gravity
	double gravity; //corresponding to "G" values on wiki. automatically calculated from level
	double effGravity; //may be more than gravity if soft drop is active
	double gravityProgress; //when above 1, moves player down and decrements by 1
	std::queue<int> blockQueue; //queue of "next blocks"
	int blockBag[NUM_BLOCK_TYPES]; //used as part of randomizer. one of each block
	int lockProgress;
	int heldBlock;
	bool holdUsed;

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