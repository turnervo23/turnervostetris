#include "Player.h"
#include "Playfield.h"
#include "UI.h"
#include "Game.h"
#include "Texture.h"

extern SDL_Renderer* gRenderer;

//Constructor - sets all initial values for player
Player::Player(Playfield* p) {
	playfield = p;
	score = 0;
	level = 1;
	gravity = 1.0 / (60.0 * pow(0.8 - (level - 1)*0.007, level - 1));
	effGravity = gravity;
	for (int i = 0; i < NUM_BLOCK_TYPES; i++) {
		blockBag[i] = i;
	}
	while (blockQueue.size() < NUM_NEXT_PIECES) {
		addBlocksToQueue();
	}

	heldBlock = NO_BLOCK;
	holdUsed = false;
	moveProgress = 0;
	numLinesCleared = 0;
	playfield->player = this;
	tSpin = false;
	combo = -1;
	tstTwist = false;

	//Temporary. Starts drop upon creation
	//startDrop();
	//startDrop() is now called by playfield->startGame()
}

//Spawn player block at top of the screen as "next" piece
void Player::startDrop() {
	getNextBlock();
	spawnBlock();
	holdUsed = false; //only if not called by holdBlock()
}

//Sets the orientation of the block, setting blockCoords in the process
//This basically stores all the block location info
void Player::setOrientation(int o) {
	orientation = o;
	switch (o) {
	case 0:
		switch (type) {
		case I_BLOCK:
			setBlockCoords(-1, 0, 0, 0, 1, 0, 2, 0);
			break;
		case J_BLOCK:
			setBlockCoords(-1, -1, -1, 0, 0, 0, 1, 0);
			break;
		case L_BLOCK:
			setBlockCoords(1, -1, -1, 0, 0, 0, 1, 0);
			break;
		case O_BLOCK:
			setBlockCoords(0, -1, 1, -1, 0, 0, 1, 0);
			break;
		case S_BLOCK:
			setBlockCoords(0, -1, 1, -1, -1, 0, 0, 0);
			break;
		case T_BLOCK:
			setBlockCoords(0, -1, -1, 0, 0, 0, 1, 0);
			break;
		case Z_BLOCK:
			setBlockCoords(-1, -1, 0, -1, 0, 0, 1, 0);
			break;
		}
		break;
	case 1:
		switch (type) {
		case I_BLOCK:
			setBlockCoords(0, -1, 0, 0, 0, 1, 0, 2);
			break;
		case J_BLOCK:
			setBlockCoords(0, -1, 1, -1, 0, 0, 0, 1);
			break;
		case L_BLOCK:
			setBlockCoords(0, -1, 0, 0, 0, 1, 1, 1);
			break;
		case O_BLOCK:
			setBlockCoords(0, -1, 1, -1, 0, 0, 1, 0);
			break;
		case S_BLOCK:
			setBlockCoords(0, -1, 0, 0, 1, 0, 1, 1);
			break;
		case T_BLOCK:
			setBlockCoords(0, -1, 0, 0, 1, 0, 0, 1);
			break;
		case Z_BLOCK:
			setBlockCoords(1, -1, 0, 0, 1, 0, 0, 1);
			break;
		}
		break;
	case 2:
		switch (type) {
		case I_BLOCK:
			setBlockCoords(-2, 0, -1, 0, 0, 0, 1, 0);
			break;
		case J_BLOCK:
			setBlockCoords(-1, 0, 0, 0, 1, 0, 1, 1);
			break;
		case L_BLOCK:
			setBlockCoords(-1, 0, 0, 0, 1, 0, -1, 1);
			break;
		case O_BLOCK:
			setBlockCoords(0, -1, 1, -1, 0, 0, 1, 0);
			break;
		case S_BLOCK:
			setBlockCoords(0, 0, 1, 0, -1, 1, 0, 1);
			break;
		case T_BLOCK:
			setBlockCoords(-1, 0, 0, 0, 1, 0, 0, 1);
			break;
		case Z_BLOCK:
			setBlockCoords(-1, 0, 0, 0, 0, 1, 1, 1);
			break;
		}
		break;
	case 3:
		switch (type) {
		case I_BLOCK:
			setBlockCoords(0, -2, 0, -1, 0, 0, 0, 1);
			break;
		case J_BLOCK:
			setBlockCoords(0, -1, 0, 0, -1, 1, 0, 1);
			break;
		case L_BLOCK:
			setBlockCoords(-1, -1, 0, -1, 0, 0, 0, 1);
			break;
		case O_BLOCK:
			setBlockCoords(0, -1, 1, -1, 0, 0, 1, 0);
			break;
		case S_BLOCK:
			setBlockCoords(-1, -1, -1, 0, 0, 0, 0, 1);
			break;
		case T_BLOCK:
			setBlockCoords(0, -1, -1, 0, 0, 0, 0, 1);
			break;
		case Z_BLOCK:
			setBlockCoords(0, -1, -1, 0, 0, 0, -1, 1);
			break;
		}
		break;
	}
}

//Helper function: sets the blockCoords given 8 ints corresponding to the 4x2 array
//This exists so I don't have to type out 8 blockCoords[i][j] assignments every time
void Player::setBlockCoords(int a, int b, int c, int d, int e, int f, int g, int h) {
	blockCoords[0][0] = a;
	blockCoords[0][1] = b;
	blockCoords[1][0] = c;
	blockCoords[1][1] = d;
	blockCoords[2][0] = e;
	blockCoords[2][1] = f;
	blockCoords[3][0] = g;
	blockCoords[3][1] = h;
}

//Spawn block at initial coordinates
void Player::spawnBlock() {
	x = 4; y = -1;
	setOrientation(0);
	gravityProgress = 0.0;
	lockProgress = 0;
	groundActions = 0;

	if (isColliding()) { //game over if spawns in another block's place ("block out")
		playfield->endGame();
	}
	else if (!isTouchingGround()) { //immediately move one down if nothing in its path
		y++;
	}
}

//Move the player left
void Player::moveLeft() {
	x -= 1;
	if (isColliding()) { //undo if invalid
		x += 1;
	}
	else {
		lockProgress = 0;
		tstTwist = false;
		prevAction = MOVE;
		if (isTouchingGround()) {
			groundActions += 1; //only so many ground actions before lock
		}
	}
}

//Move the player right
void Player::moveRight() {
	x += 1;
	if (isColliding()) { //undo if invalid
		x -= 1;
	}
	else {
		lockProgress = 0;
		tstTwist = false;
		prevAction = MOVE;
		if (isTouchingGround()) {
			groundActions += 1; //only so many ground actions before lock
		}
	}
}

//Speed up gravity temporarily (called on player input)
void Player::applySoftDrop(bool status) {
	softDropping = status;
}

//Land the piece instantly and start the next drop
void Player::hardDrop() {
	if (!playfield->suspended) {
		while (!isTouchingGround()) {
			y++;
			tstTwist = false;
			prevAction = DROP;
			addScore(2); //points for hard dropping			
		}
		landBlock();
	}
}

//Call all rendering functions
void Player::render() {
	if (!playfield->countdown && !playfield->gameOver) {
		renderPlayer();
		renderGhostBlock();
	}
	renderNextBlocks();
	renderHeldBlock();
}

//Render player's blocks
void Player::renderPlayer() {
	SDL_RenderSetClipRect(gRenderer, &playfield->playfieldClip);
	for (int i = 0; i < NUM_PLAYER_BLOCKS; i++) {
		playfield->textures[type].render(playfield->x + 32 * (x + blockCoords[i][0]),
			playfield->y + 32 * (y + blockCoords[i][1]));
		playfield->playerGlowTexture.render(playfield->x + 32 * (x + blockCoords[i][0]),
			playfield->y + 32 * (y + blockCoords[i][1]));
	}
}

//Update player status. Called once per frame
void Player::update() {
	//This is outside of the suspended check so autorepeat can be buffered while suspended.
	//There is another suspended check in the function for actual movement
	processMovement();

	if (!playfield->suspended) {
		applyGravity();
		applyLock();
	}
}

//Automatically move player down
void Player::applyGravity() {
	//Update gravity value
	gravity = 1.0 / (60.0 * pow(0.8 - (level - 1)*0.007, level - 1));
	if (softDropping && gravity < SOFT_DROP_G)
		effGravity = SOFT_DROP_G;
	else
		effGravity = gravity;

	//Move player down if possible
	gravityProgress += effGravity;
	while (gravityProgress >= 1.0) {
		if (!isTouchingGround()) {
			y += 1;
			tstTwist = false;
			prevAction = DROP;

			//Points for soft dropping
			if (softDropping == true) {
				addScore(1);
			}
		}
		gravityProgress -= 1.0;
	}
}

//Rotates the player's piece left (counterclockwise)
void Player::rotateLeft() {
	if (!playfield->suspended) {
		int prev = orientation;
		if (type == I_BLOCK) { //move center of rotation. specific for I block
			switch (prev) { //based on prev orientation
			case 0:
				y += 1;
				break;
			case 1:
				x -= 1;
				break;
			case 2:
				y -= 1;
				break;
			case 3:
				x += 1;
				break;
			}
		}

		//set new orientation
		orientation -= 1;
		if (orientation == -1) {
			orientation = 3;
		}
		setOrientation(orientation);

		tstTwist = false; //For 3rd T-spin mini test
		if (isColliding() && !wallKick(prev)) { //wall kick or undo rotation
			rotateRight();
		}
		else {
			lockProgress = 0; //reset lock progress if success
			prevAction = ROTATE;
			if (isTouchingGround()) {
				groundActions += 1; //only so many ground actions before lock
			}
		}
	}
}

//Rotates the player's piece right (clockwise)
void Player::rotateRight() {
	if (!playfield->suspended) {
		int prev = orientation;
		if (type == I_BLOCK) { //move center of rotation. specific for I block
			switch (prev) { //based on prev orientation
			case 0:
				x += 1;
				break;
			case 1:
				y += 1;
				break;
			case 2:
				x -= 1;
				break;
			case 3:
				y -= 1;
				break;
			}
		}

		//set new orientation
		orientation += 1;
		if (orientation == 4) {
			orientation = 0;
		}
		setOrientation(orientation);

		tstTwist = false; //For 3rd T-spin mini test
		if (isColliding() && !wallKick(prev)) { //wall kick or undo rotation
			rotateLeft();
		}
		else {
			lockProgress = 0; //reset lock delay if success
			prevAction = ROTATE;
			if (isTouchingGround()) {
				groundActions += 1; //only so many ground actions before lock
			}
		}
	}
}

//Returns true if player blocks are colliding with the environment blocks OR out-of-bounds
bool Player::isColliding() {
	bool result = false;
	for (int i = 0; i < NUM_PLAYER_BLOCKS; i++) {
		if (playfield->grid[20 + y + blockCoords[i][1]][x + blockCoords[i][0]] != NO_BLOCK //playfield block collision
			|| 20 + y + blockCoords[i][1] >= FIELD_HEIGHT //below bottom of screen
			|| x + blockCoords[i][0] <= -1 //past left of screen
			|| x + blockCoords[i][0] >= FIELD_WIDTH) { //past right of screen
			result = true;
		}
	}
	return result;
}

//Attempt to perform a wall kick based on SRS table data. Return true if success
//parameter is previous orientation. needed for wall kick table
//NOTE: coords on wiki have inverted y coords from my system.
bool Player::wallKick(int prev) {
	int origX = x; //save old values for reverting after each test
	int origY = y;

	//determine which row of table to use
	int row = -1;
	if (prev == 0 && orientation == 1)
		row = 0;
	else if (prev == 1 && orientation == 0)
		row = 1;
	else if (prev == 1 && orientation == 2)
		row = 2;
	else if (prev == 2 && orientation == 1)
		row = 3;
	else if (prev == 2 && orientation == 3)
		row = 4;
	else if (prev == 3 && orientation == 2)
		row = 5;
	else if (prev == 3 && orientation == 0)
		row = 6;
	else if (prev == 0 && orientation == 3)
		row = 7;
	
	switch (type) {
	case J_BLOCK:
	case L_BLOCK:
	case S_BLOCK:
	case T_BLOCK:
	case Z_BLOCK:
		for (int col = 0; col < 4; col++) {
			x += wallKickData_JLSTZ[row][col][0];
			y += wallKickData_JLSTZ[row][col][1];
			if (!isColliding()) { //if no collision after new coord check, return true and use new coords
				//T-spin mini check #3 (TST twist)
				if (type == T_BLOCK && col == 3) {
					tstTwist = true;
				}
				return true;
			}
			x = origX;
			y = origY;
		}
		break;
	case I_BLOCK:
		for (int col = 0; col < 4; col++) {
			x += wallKickData_I[row][col][0];
			y += wallKickData_I[row][col][1];
			if (!isColliding()) { return true; } //if no collision after new coord check, return true and use new coords
			x = origX;
			y = origY;
		}
		break;
	}

	return false;
}

//debug function. pick which piece you want!
void Player::_debug_cyclePiece() {
	type += 1;
	if (type == 7)
		type = 0;
	setOrientation(orientation);
}

//Adds randomized blocks to queue
void Player::addBlocksToQueue() {
	unsigned int seed = std::chrono::system_clock::now().time_since_epoch().count();
	std::shuffle(&blockBag[0], &blockBag[NUM_BLOCK_TYPES - 1], std::default_random_engine(seed));
	for (int i = 0; i < NUM_BLOCK_TYPES; i++) {
		blockQueue.push(blockBag[i]);
	}
}

//Removes the next block from the queue, and sets the player's type to it
void Player::getNextBlock() {
	type = blockQueue.front();
	blockQueue.pop();
	if (blockQueue.size() < NUM_NEXT_PIECES) {
		addBlocksToQueue();
	}
}

//Renders the next pieces. Right side of playfield. First one larger than the rest
//This function is super inefficient...cycles queue and sets NUM_NEXT_PIECES blockCoords every frame
void Player::renderNextBlocks() {
	int needed[NUM_NEXT_PIECES]; //the ones we need to render
	int queueLength = blockQueue.size();

	//have to cycle the whole queue to get ones in the middle...not super efficient
	for (int i = 0; i < queueLength; i++) {
		if (i < NUM_NEXT_PIECES) {
			needed[i] = blockQueue.front();
		}
		blockQueue.push(blockQueue.front());
		blockQueue.pop();
	}

	//I'm using player's blockCoords, type, and orientation temporarily to render the next pieces.
	//These variables save those values so we can set them back at the end
	int savedType = type;
	int savedOri = orientation;

	SDL_RenderSetClipRect(gRenderer, &playfield->nextBlockClip);
	orientation = 0;
	//First piece, rendered 3/4 size
	type = needed[0];
	setOrientation(orientation);
	for (int j = 0; j < NUM_PLAYER_BLOCKS; j++) {
		playfield->textures[needed[0]].render(playfield->x + (32 * FIELD_WIDTH) + (24 * blockCoords[j][0]) + 56,
			                                  playfield->y + (24 * blockCoords[j][1]) + 56,
			                                  24, 24);
	}

	//Rest of the pieces, rendered half-size
	for (int i = 1; i < NUM_NEXT_PIECES; i++) {
		type = needed[i];
		setOrientation(orientation);
		for (int j = 0; j < NUM_PLAYER_BLOCKS; j++) {
			playfield->textures[needed[i]].render(playfield->x + (32 * FIELD_WIDTH) + (16 * blockCoords[j][0]) + 56,
				                                  playfield->y + (16 * blockCoords[j][1]) + 72 + (48 * i),
				                                  16, 16);
		}
	}

	//Returning player vars to original values
	type = savedType;
	orientation = savedOri;
	setOrientation(orientation);

	//Draw 8 px box around next piece (shouldn't really be in this class)
	SDL_SetRenderDrawColor(gRenderer, 0xb0, 0xb0, 0xb0, 0xFF); //light gray
	for (int i = 0; i < 8; i++) {
		SDL_RenderDrawLine(gRenderer, playfield->x + (32 * FIELD_WIDTH),
			                          playfield->y - 8 + i,
			                          playfield->x + (32 * FIELD_WIDTH) + 152,
			                          playfield->y - 8 + i); //top
		SDL_RenderDrawLine(gRenderer, playfield->x + (32 * FIELD_WIDTH),
			                          playfield->y + 360 + i,
			                          playfield->x + (32 * FIELD_WIDTH) + 152,
			                          playfield->y + 360 + i); //bottom
		SDL_RenderDrawLine(gRenderer, playfield->x + (32 * FIELD_WIDTH) + 152 - i,
			                          playfield->y - 8,
			                          playfield->x + (32 * FIELD_WIDTH) + 152 - i,
			                          playfield->y + 360); //right
	}
	//"NEXT" text is rendered by UI function. Too much of a hassle to introduce another text object here
}

//Render the held piece, to the left of top of playfield
void Player::renderHeldBlock() {
	SDL_RenderSetClipRect(gRenderer, &playfield->heldBlockClip);

	//Only render the block itself if there is one. Crashes otherwise
	if (heldBlock != -1) {
		//Using player's blockCoords, type, and orientation temporarily to render the held block.
		//These variables save those values so we can set them back at the end
		int savedType = type;
		int savedOri = orientation;

		//Gray out if hold used already
		if (holdUsed) {
			playfield->textures[heldBlock].setColor(127, 127, 127);
		}
		orientation = 0;
		//Rendered 3/4 size
		type = heldBlock;
		setOrientation(orientation);
		for (int j = 0; j < NUM_PLAYER_BLOCKS; j++) {
			playfield->textures[heldBlock].render(playfield->x + (24 * blockCoords[j][0]) - 104,
				playfield->y + (24 * blockCoords[j][1]) + 64,
				24, 24);
		}

		//Returning player vars to orig values
		playfield->setTextureColorByBlockType(playfield->textures[heldBlock], heldBlock);
		type = savedType;
		orientation = savedOri;
		setOrientation(orientation);
	}
	
	//Draw 8 px box around held piece (shouldn't really be in this class)
	SDL_SetRenderDrawColor(gRenderer, 0xb0, 0xb0, 0xb0, 0xFF); //light gray
	for (int i = 0; i < 8; i++) {
		SDL_RenderDrawLine(gRenderer, playfield->x,
			                          playfield->y - 8 + i,
			                          playfield->x - 152,
			                          playfield->y - 8 + i); //top
		SDL_RenderDrawLine(gRenderer, playfield->x,
			                          playfield->y + 96 + i,
			                          playfield->x - 152,
			                          playfield->y + 96 + i); //bottom
		SDL_RenderDrawLine(gRenderer, playfield->x - 152 + i,
			                          playfield->y - 8,
			                          playfield->x - 152 + i,
			                          playfield->y + 96); //left
	}
	//"HOLD" text is rendered by UI function. Too much of a hassle to introduce another text object here
}

//Returns true if at least one of the player's blocks is directly above a playfield block or the bottom of the playfield
bool Player::isTouchingGround() {
	bool result = false;
	for (int i = 0; i < NUM_PLAYER_BLOCKS; i++) {
		if (playfield->grid[20 + y + blockCoords[i][1] + 1][x + blockCoords[i][0]] != NO_BLOCK //playfield block below
			|| 20 + y + blockCoords[i][1] + 1 == 40) { //or at bottom of screen
			result = true;
		}
	}
	return result;
}

//Increases progress towards piece locking in if touching the ground, or lands piece if finished
void Player::applyLock() {
	if (Player::isTouchingGround()) {
		lockProgress++;
	}

	if (lockProgress >= LOCK_SPEED || groundActions >= MAX_GROUND_ACTIONS) {
		landBlock();
		lockProgress = 0;
	}
}

//Converts player blocks to playfield blocks and starts new drop
void Player::landBlock() {
	int blocksAboveVisibleField = 0; //if 4 blocks are above visible playing field after the loop, "lock out" game over
	for (int i = 0; i < NUM_PLAYER_BLOCKS; i++) {
		playfield->grid[20 + y + blockCoords[i][1]][x + blockCoords[i][0]] = type; //sets playfield blocks

		if (20 + y + blockCoords[i][1] < 20) { //checks if block is above the visible playing field
			blocksAboveVisibleField += 1;
		}
	}

	//I think this caused the screen flash bug. Seems to work fine without this line now
	//render(); //render again. otherwise there's 1 frame of it missing

	//Land successful
	if (blocksAboveVisibleField < NUM_PLAYER_BLOCKS) {
		//start lock flash animation
		playfield->lockFlashing = true;
		playfield->lockFlashStartFrame = playfield->game->getCurFrame();
		for (int i = 0; i < NUM_PLAYER_BLOCKS; i++) {
			playfield->lockFlashX[i] = x + blockCoords[i][0];
			playfield->lockFlashY[i] = y + blockCoords[i][1];
		}

		//T-spin check
		if (type == T_BLOCK
			&& prevAction == ROTATE
			&& tSpinThreeCornersOccupied()) {
			//T-spin mini
			/*
			T-spin mini notes - from http://kitaru.1101b.com/tc/mini.html:
			"a t-spin is mini if there is an open corner on the point-end,
			it clears one line or less, or reached its final(x, y) position by way of the t-spin triple wall kick"
			I think the last one is the opposite - and is NOT a mini if used TST twist
			*/
			if (tSpinUnoccupiedByPoint()
				&& tstTwist == false) {
				tSpinMini = true;
				tSpin = false;
			}
			//Regular T-spin
			else {
				tSpin = true;
				tSpinMini = false;
			}
		}
		else {
			tSpin = false;
			tSpinMini = false;
		}

		//Check for line clear
		playfield->checkLineClear();

		//If no line was cleared, start next drop.
		//Otherwise drop is started by playfield->renderLineClear()
		if (!playfield->lineClearing) {
			startDrop();
		}
	}
	else {
		playfield->endGame(); //player locked out, end game
	}
}

//debug only - clear playfield of all blocks
void Player::_debug_clearPlayfield() {
	for (int r = 0; r < FIELD_HEIGHT; r++) {
		for (int c = 0; c < FIELD_WIDTH; c++) {
			playfield->grid[r][c] = NO_BLOCK;
		}
	}
}

//Switches current block with held block and restarts the drop.
//Only usable if it hasn't been used since the last piece landed.
void Player::holdBlock() {
	if (!holdUsed && !playfield->suspended) {
		int temp = heldBlock;
		heldBlock = type;

		if (temp != NO_BLOCK) { //if no held block existed, use next piece
			type = temp;
			spawnBlock();
		}
		else {
			startDrop();
		}

		holdUsed = true;
	}
}

//Render ghost block at the playfield floor with 50% transparency
void Player::renderGhostBlock() {
	int tempY = y; //save player's actual position
	while (!isTouchingGround()) {
		y += 1;
	}
	playfield->textures[type].setAlpha(127);
	renderPlayer();
	playfield->textures[type].setAlpha(255);
	y = tempY;
}

//Track whether player is holding left or right (for autorepeat)
void Player::startMoveLeft() { holdingLeft = true; }
void Player::startMoveRight() { holdingRight = true; }
void Player::endMoveLeft() {
	holdingLeft = false;
	moveProgress = 0;
}
void Player::endMoveRight() {
	holdingRight = false;
	moveProgress = 0;
}

//Moves left or right if moveProgress is at a certain value.
//It moves when left/right is first pressed, and every x frames after a certain amount of time has passed
void Player::processMovement() {
	if (holdingLeft == true || holdingRight == true) {
		moveProgress += 1;
	}

	if (!playfield->suspended) {
		if (moveProgress == 1 || (moveProgress >= 10 && moveProgress % 2 == 0)) {
			if (holdingLeft == true)
				moveLeft();
			else if (holdingRight == true) {
				moveRight();
			}
		}
	}
}

//Adds s to score
void Player::addScore(int s) {
	score += s;
}

//Returns the number of lines cleared
int Player::getNumLinesCleared() {
	return numLinesCleared;
}

//Increments the number of lines cleared
void Player::incrementNumLinesCleared() {
	numLinesCleared++;
	if (numLinesCleared % 10 == 0 && numLinesCleared <= 140) { //increment level every 10 lines cleared, up to 15
		level += 1;
	}
}

//Checks whether 3 corners around a T block are occupied. One of the conditions for a T-spin
bool Player::tSpinThreeCornersOccupied() {
	int numCornersOccupied = 0;
	int cornerCoords[4][2] = { {-1, -1}, {1, -1}, {-1, 1}, {1, 1} };

	//Checks each corner to see if it is occupied
	for (int i = 0; i < 4; i++) {
		if (playfield->grid[20 + y + cornerCoords[i][1]][x + cornerCoords[i][0]] != NO_BLOCK
			|| 20 + y + cornerCoords[i][1] >= FIELD_HEIGHT //below bottom of screen
			|| x + cornerCoords[i][0] <= -1 //past left of screen
			|| x + cornerCoords[i][0] >= FIELD_WIDTH) { //past right of screen)
			numCornersOccupied += 1;
		}
	}

	if (numCornersOccupied == 3) {
		return true;
	}
	else {
		return false;
	}
}

//Adds one to the player's combo. Called by playfield upon line clear.
void Player::incrementCombo() {
	combo++;
}

//Resets the combo to -1. Called by playfield after land with no line clear
void Player::resetCombo() {
	combo = -1;
}

//Checks if one of the spaces next to the T block's "point" are empty. 1st T-spin mini check
bool Player::tSpinUnoccupiedByPoint() {
	switch (orientation) {
	case 0:
		if (playfield->grid[20 + y - 1][x - 1] == NO_BLOCK
			|| playfield->grid[20 + y - 1][x + 1] == NO_BLOCK) {
			return true;
		}
		break;
	case 1:
		if (playfield->grid[20 + y - 1][x + 1] == NO_BLOCK
			|| playfield->grid[20 + y + 1][x + 1] == NO_BLOCK) {
			return true;
		}
		break;
	case 2:
		if (playfield->grid[20 + y + 1][x - 1] == NO_BLOCK
			|| playfield->grid[20 + y + 1][x + 1] == NO_BLOCK) {
			return true;
		}
		break;
	case 3:
		if (playfield->grid[20 + y - 1][x - 1] == NO_BLOCK
			|| playfield->grid[20 + y + 1][x - 1] == NO_BLOCK) {
			return true;
		}
		break;
	}
	return false;
}