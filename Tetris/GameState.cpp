#include "GameState.h"

GameState::GameState(GameState** gsp) {
	gameStatePtr = gsp;
}

void GameState::changeState(GameState *state) {
	gameStatePtr = &state;
}