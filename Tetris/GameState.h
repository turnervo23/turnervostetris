#pragma once
#ifndef GAMESTATE_H
#define GAMESTATE_H

#include <stack>

class GameState {
public:
	GameState(GameState**);
	virtual void update() = 0;
	virtual bool quitting() = 0;
	void changeState(GameState* state);
private:
	GameState** gameStatePtr; //points to self while active
};

#endif