//List of constants which can be used by multiple classes
#pragma once
#ifndef CONSTANTS_H
#define CONSTANTS_H

//Screen dimension constants
const int SCREEN_WIDTH = 672; //320 playfield, 16 border, 168 next blocks, 168 hold blocks?
const int SCREEN_HEIGHT = 720;

enum GameStates {
	MENU,
	GAME
};

const int FIELD_WIDTH = 10;
const int FIELD_HEIGHT = 40;
const int VISIBLE_FIELD_HEIGHT = 20;
const int LINE_CLEAR_DELAY = 30; //in frames
const int LINE_CLEAR_FLASH_FREQ = 4;
const int LOCK_FLASH_TIME = 10; //in frames
const int NUM_PLAYER_BLOCKS = 4;
const int NUM_BLOCK_TYPES = 7;
const int NUM_DIMENSIONS = 2;
const int NUM_NEXT_PIECES = 6;
const double SOFT_DROP_G = 0.5; //moves down this amount per frame
const int LOCK_SPEED = 40; //in frames
const int MAX_GROUND_ACTIONS = 15;
const int GAME_OVER_TEXT_DELAY = 60;
const int CLEAR_TYPE_DISPLAY_TIME = 120;
const int COUNTDOWN_FRAMES_PER_STAGE = 60;
const int ALL_CLEAR_DISPLAY_TIME = 120;

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

#endif