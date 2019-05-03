#include "Player.h"

Player::Player(Playfield* p) {
	playfield = p;
	level = 1;
	gravity = 1.0 / (60.0 * pow(0.8 - (level - 1)*0.007, level - 1));
	effGravity = gravity;
	for (int i = 0; i < NUM_BLOCK_TYPES; i++) {
		blockBag[i] = i;
	}
	while (blockQueue.size() < NUM_NEXT_PIECES) {
		addBlocksToQueue();
	}

	heldBlock = -1; //no block
	holdUsed = false;

	moveProgress = 0;
}

//Spawn player block at top of the screen as "next" piece
void Player::startDrop() {
	getNextBlock();
	spawnBlock();
	setOrientation(0);
	gravityProgress = 0;
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
	if (!isTouchingGround()) { //immediately move one down if nothing in its path
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
	}
}

//Speed up gravity temporarily (called on player input)
void Player::applySoftDrop(bool yes) {
	if (yes && gravity < SOFT_DROP_G)
		effGravity = SOFT_DROP_G;
	else
		effGravity = gravity;
}

//Land the piece instantly and start the next drop
void Player::hardDrop() {
	while (!isTouchingGround()) {
		y++;
	}
	landBlock();
}

//Call all rendering functions
void Player::render() {
	renderPlayer();
	renderNextBlocks();
	renderHeldBlock();
	renderGhostBlock();
}

//Render player's blocks
void Player::renderPlayer() {
	SDL_RenderSetClipRect(gRenderer, &playfield->playfieldClip);
	for (int i = 0; i < NUM_PLAYER_BLOCKS; i++) {
		playfield->textures[type].render(playfield->x + 32 * (x + blockCoords[i][0]),
			playfield->y + 32 * (y + blockCoords[i][1]));
	}
}

//Update player status. Called once per frame
void Player::update() {
	if (playfield->clearing == false) {
		processMovement();
		applyGravity();
		applyLock();
	}
}

//Automatically move player down
void Player::applyGravity() {
	gravityProgress += effGravity;
	while (gravityProgress >= 1.0) {
		if (!isTouchingGround()) {
			y += 1;
		}
		gravityProgress -= 1.0;
	}
}

//Rotates the player's piece left (counterclockwise)
void Player::rotateLeft() {
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

	if (isColliding() && !wallKick(prev)) { //wall kick or undo rotation
		rotateRight();
	}
	else {
		lockProgress = 0; //reset lock progress if success
	}
}

//Rotates the player's piece right (clockwise)
void Player::rotateRight() {
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

	if (isColliding() && !wallKick(prev)) { //wall kick or undo rotation
		rotateLeft();
	}
	else {
		lockProgress = 0; //reset lock delay if success
	}
}

//Returns true if player blocks are colliding with the environment blocks OR out-of-bounds
bool Player::isColliding() {
	bool result = false;
	for (int i = 0; i < NUM_PLAYER_BLOCKS; i++) {
		if (playfield->grid[20 + y + blockCoords[i][1]][x + blockCoords[i][0]] != NO_BLOCK //playfield block collision
			|| 20 + y + blockCoords[i][1] >= 40 //below bottom of screen
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
			if (!isColliding()) { return true; } //if no collision after new coord check, return true and use new coords
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
			                                  playfield->y + (24 * blockCoords[j][1]) + 40,
			                                  24, 24);
	}

	//Rest of the pieces, rendered half-size
	for (int i = 1; i < NUM_NEXT_PIECES; i++) {
		type = needed[i];
		setOrientation(orientation);
		for (int j = 0; j < NUM_PLAYER_BLOCKS; j++) {
			playfield->textures[needed[i]].render(playfield->x + (32 * FIELD_WIDTH) + (16 * blockCoords[j][0]) + 56,
				                                  playfield->y + (16 * blockCoords[j][1]) + 56 + (48 * i),
				                                  16, 16);
		}
	}

	//Returning player vars to original values
	type = savedType;
	orientation = savedOri;
	setOrientation(orientation);
}

//Render the held piece, to the left of top of playfield
void Player::renderHeldBlock() {
	//skip if no held block
	if (heldBlock == -1) {
		return;
	}

	//I'm using player's blockCoords, type, and orientation temporarily to render the held block.
	//These variables save those values so we can set them back at the end
	int savedType = type;
	int savedOri = orientation;

	SDL_RenderSetClipRect(gRenderer, &playfield->heldBlockClip);
	orientation = 0;
	//Rendered 3/4 size
	type = heldBlock;
	setOrientation(orientation);
	for (int j = 0; j < NUM_PLAYER_BLOCKS; j++) {
		playfield->textures[heldBlock].render(playfield->x + (24 * blockCoords[j][0]) - 104,
			                                  playfield->y + (24 * blockCoords[j][1]) + 40,
			                                  24, 24);
	}

	//Returning player vars to orig values
	type = savedType;
	orientation = savedOri;
	setOrientation(orientation);
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

	if (lockProgress >= LOCK_SPEED) {
		landBlock();
		lockProgress = 0;
	}
}

//Converts player blocks to playfield blocks and starts new drop
void Player::landBlock() {
	for (int i = 0; i < NUM_PLAYER_BLOCKS; i++) {
		playfield->grid[20 + y + blockCoords[i][1]][x + blockCoords[i][0]] = type; //sets playfield blocks
	}
	render(); //render again. otherwise there's 1 frame of it missing
	startDrop();
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
	if (!holdUsed) {
		int temp = heldBlock;
		heldBlock = type;

		if (temp != NO_BLOCK) { //if no held block existed, use next piece
			type = temp;
			spawnBlock();
			setOrientation(0);
			gravityProgress = 0;
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

	if (moveProgress == 1 || (moveProgress >= 10 && moveProgress % 2 == 0)) {
		if (holdingLeft == true)
			moveLeft();
		else if (holdingRight == true) {
			moveRight();
		}
	}
}