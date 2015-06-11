/**
 * Camera.cpp
 *
 */

#include "Camera.h"

Camera::Camera(int width, int height, PlayerController *playerController):
	playerController(playerController),
	camera_state(RIGHT)
{
	SDL_Rect realcamera = {0, 0, width, height};
	frame = realcamera;
}

void Camera::update()
{
	frame.y = lerp(frame.y, playerController->getLocation().y);
	frame.x = lerp(frame.x, playerController->getLocation().x);
}

int Camera::lerp(int a, int b)
{
	return int(a + (b - a) * LERP);
}

SDL_Rect Camera::getFrame()
{
	return frame;
}