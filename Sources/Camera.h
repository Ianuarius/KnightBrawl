/**
 * Camera.h
 *
 */

#ifndef __CAMERA_H_DEFINED__
#define __CAMERA_H_DEFINED__

#define LERP 0.1

#include "SDL.h"
#include "PlayerController.h"

class Camera
{

public:
	Camera(int width, int height, std::vector<PlayerController *> *playerControllers);
	void update();
	int getState();
	SDL_Rect getFrame();
	float facing_threshold;
	float pan_threshold;
	bool movementLocked();

private:
	std::vector<PlayerController *> *playerControllers;
	enum Direction {RIGHT, LEFT};
	SDL_Rect frame;
	int camera_state;
	int lerp(int a, int b);
};


#endif //__CAMERA_H_DEFINED__