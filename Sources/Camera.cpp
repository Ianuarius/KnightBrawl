/**
 * Camera.cpp
 *
 */

#include "Camera.h"

Camera::Camera(int width, int height):
	facing(RIGHT)
{
	SDL_Rect realcamera = {0, 0, width, height};
	frame = realcamera;
}

void Camera::lock(PlayerActor *player) 
{
	lockedPlayer = player;
}

void Camera::update()
{
	frame.x = frame.x + (lockedPlayer->position.x - frame.x) * LERP;
	frame.y = frame.y + (lockedPlayer->position.y - frame.y) * LERP;
	/*
	camera.frame.x + (player1.position.x - camera.frame.x) * lerp, 
	camera.frame.y + (player1.position.y - camera.frame.y) * lerp
	*/
}