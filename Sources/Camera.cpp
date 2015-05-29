/**
 * Camera.cpp
 *
 */

#include "Camera.h"

Camera::Camera(int width, int height)
{
	SDL_Rect realcamera = {0, 0, width, height};
	frame = realcamera;
}

void Camera::update(int x, int y)
{
	frame.x = x;
	frame.y = y;
}