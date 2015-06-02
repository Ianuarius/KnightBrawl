/**
 * Camera.cpp
 *
 */

#include "Camera.h"

Camera::Camera(int width, int height, PlayerController *playerController):
	facing(RIGHT),
	playerController(playerController),
	offset(0)
{
	SDL_Rect realcamera = {0, 0, width, height};
	frame = realcamera;
}

/*
UPDATE CAMERA
- If player is in area FL < X < FR
- If player is in X < PL
- If player is in X > PR

DON'T UPDATE
- If player is in area PL < X < FL
- If player is in area FR < X < PR

*/

void Camera::update()
{
	facing = playerController->getDirection();
	/*
	if (facing == RIGHT) {
		
		if (playerController->getLocation().x > (frame.x + (frame.w * 0.75f))) {
			offset = (frame.w * 0.4f) - (frame.w / 2);
		}

	} else if (facing == LEFT) {
		
		if (playerController->getLocation().x < (frame.x + (frame.w * 0.25f))) {
			offset = (frame.w * 0.6f) - (frame.w / 2);
		}

	}
	*/

	if (playerController->getLocation().x < (frame.x + (frame.w * 0.2f))) {
		frame.x = playerController->getLocation().x - 0.2f;
		offset = 10;
	} else if (playerController->getLocation().x > (frame.x + (frame.w * 0.8f))) {
		frame.x = playerController->getLocation().x - 0.8f;
		offset = -10;
	}

	frame.y = playerController->getLocation().y;
	/*
	if (lockedPlayer->position.x > (frame.w * FACING_L) &&
	    lockedPlayer->position.x < (frame.w * FACING_R)) {
		//PÄIVITÄ KAMERA

			//frame.x = frame.x + (lockedPlayer->position.x - 
			//	(frame.x * lockedPlayer->facing_position)) * LERP;
	
	} else if (lockedPlayer->position.x < PAN_TR_L ||
			   lockedPlayer->position.x > PAN_TR_R) {
			//TARKISTA SUUNTA
		if (lockedPlayer->facing == RIGHT) {

		}

	} else {
		// ÄLÄ PÄIVITÄ
	}

	*/
}

int Camera::getOffset()
{
	return offset;
}

SDL_Rect Camera::getFrame()
{
	return frame;
}


	// frame.y = frame.y + (lockedPlayer->position.y - frame.y) * LERP;

	/*
	if (facing == RIGHT) {
		facing_threshold = FACING_L;
		pan_threshold = PAN_TR_L;
	} else {
		facing_threshold = FACING_R;
		pan_threshold = PAN_TR_R;
	}

	if (lockedPlayer->facing == facing) {
		if (lockedPlayer->position.x >= frame.w * 
	} else {

	}
	*/

	/*
	camera.frame.x + (player1.position.x - camera.frame.x) * lerp, 
	camera.frame.y + (player1.position.y - camera.frame.y) * lerp
	*/