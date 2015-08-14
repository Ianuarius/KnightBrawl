/**
 * Camera.cpp
 *
 */

#include "Camera.h"

Camera::Camera(int width, int height, std::vector<PlayerController *> *playerControllers, int players):
	playerControllers(playerControllers),
	camera_state(RIGHT),
	players(players)
{
	int max_x = 0, min_x = 100000;
	int max_y = 0, min_y = 100000;
	int x_average = 0, y_average = 0;
	
	for (int i = 0; i < players; i++) {
		if ((*playerControllers)[i]->location.x > max_x) {
			max_x = (*playerControllers)[i]->location.x;
		}
		if ((*playerControllers)[i]->location.x < min_x) {
			min_x = (*playerControllers)[i]->location.x;
		}
		if ((*playerControllers)[i]->location.y > max_y) {
			max_y = (*playerControllers)[i]->location.y;
		}
		if ((*playerControllers)[i]->location.y < min_y) {
			min_y = (*playerControllers)[i]->location.y;
		}
	}

	x_average = ((max_x - min_x) / 2) + min_x;
	y_average = ((max_y - min_y) / 2) + min_y;
	
	SDL_Rect realcamera = {x_average - (width / 2), 
						   y_average - (height / 2),
						   width, 
						   height};
	frame = realcamera;
}

void Camera::update()
{
	int max_x = 0, min_x = 100000;
	int max_y = 0, min_y = 100000;
	int x_average = 0, y_average = 0;
	
	for (int i = 0; i < players; i++) {
		if ((*playerControllers)[i]->location.x > max_x) {
			max_x = (*playerControllers)[i]->location.x;
		}
		if ((*playerControllers)[i]->location.x < min_x) {
			min_x = (*playerControllers)[i]->location.x;
		}
		if ((*playerControllers)[i]->location.y > max_y) {
			max_y = (*playerControllers)[i]->location.y;
		}
		if ((*playerControllers)[i]->location.y < min_y) {
			min_y = (*playerControllers)[i]->location.y;
		}
	}
	
	x_average = ((max_x - min_x) / 2) + min_x;
	y_average = ((max_y - min_y) / 2) + min_y;

	frame.x = lerp(frame.x, x_average - (frame.w / 2));

	frame.y = lerp(frame.y, y_average - (frame.h / 2));
}

bool Camera::movementLocked()
{
	bool lock = false;
	for (int i = 0; i < players; i++) {
		for (int j = 0; j < players; j++) {
			if (std::abs(((*playerControllers)[i]->desired.x - 
						  (*playerControllers)[j]->desired.x)) > frame.w - 28 &&
						  (*playerControllers).size() > 1) {
				lock = true;
			}
		}
	}

	return lock;
}

int Camera::lerp(int a, int b)
{
	return int(a + (b - a) * LERP);
}

SDL_Rect Camera::getFrame()
{
	return frame;
}
