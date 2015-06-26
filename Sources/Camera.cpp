/**
 * Camera.cpp
 *
 */

#include "Camera.h"

Camera::Camera(int width, int height, std::vector<PlayerController *> *playerControllers):
	playerControllers(playerControllers),
	camera_state(RIGHT)
{
	SDL_Rect realcamera = {(((*playerControllers)[0]->location.x + (*playerControllers)[1]->location.x) / 2) - (width / 2), 
						   (((*playerControllers)[0]->location.y + (*playerControllers)[1]->location.y) / 2) - (height / 2),
						   width, 
						   height};
	frame = realcamera;
}

void Camera::update()
{
	frame.x = lerp(frame.x, (((*playerControllers)[0]->location.x + (*playerControllers)[1]->location.x) / 2) - (frame.w / 2));
	frame.y = lerp(frame.y, (((*playerControllers)[0]->location.y + (*playerControllers)[1]->location.y) / 2) - (frame.h / 2));
}

int Camera::lerp(int a, int b)
{
	return int(a + (b - a) * LERP);
}

SDL_Rect Camera::getFrame()
{
	return frame;
}

bool Camera::movementLocked()
{
	if (std::abs(((*playerControllers)[0]->desired.x - 
				  (*playerControllers)[1]->desired.x)) > frame.w - 28 &&
				  (*playerControllers).size() > 1) {
		return true;
	} else {
		return false;
	}
}