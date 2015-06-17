/**
 * Camera.cpp
 *
 */

#include "Camera.h"

Camera::Camera(int width, int height, PlayerController *playerController):
	playerController(playerController),
	camera_state(RIGHT)
{
	SDL_Rect realcamera = {playerController->boundbox.x - width/2, 
						   playerController->boundbox.y - height/2,
						   width, 
						   height};
	frame = realcamera;
}

void Camera::update()
{
	frame.x = lerp(frame.x, playerController->boundbox.x - frame.w/2);
	frame.y = lerp(frame.y, playerController->boundbox.y - frame.h/2);
}

int Camera::lerp(int a, int b)
{
	return int(a + (b - a));
}

SDL_Rect Camera::getFrame()
{
	return frame;
}