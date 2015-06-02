/**
 * Camera.cpp
 *
 */

#include "Camera.h"

Camera::Camera(int width, int height, PlayerController *playerController):
	facing(RIGHT),
	playerController(playerController)
{
	SDL_Rect realcamera = {0, 0, width, height};
	frame = realcamera;
}

void Camera::update()
{
	facing = playerController->getDirection();

	frame.x = playerController->getLocation().x;
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
	if (facing == RIGHT) {
		return -20;
	} else {
		return 20;
	}
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