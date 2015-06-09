/**
 * Camera.h
 *
 */

#ifndef __CAMERA_H_DEFINED__
#define __CAMERA_H_DEFINED__

#define FACING_L 0.4
#define FACING_R 0.6
#define PAN_TR_L 0.3
#define PAN_TR_R 0.7
#define LERP 0.1

#include "SDL.h"
#include "PlayerController.h"

class Camera
{

public:
	Camera(int width, int height, PlayerController *playerController);
	void update();
	int getOffset();
	SDL_Rect getFrame();
	int facing;
	float facing_threshold;
	float pan_threshold;

private:
	PlayerController *playerController;
	enum Direction {RIGHT, LEFT};
	enum States {RIGHT_STATE, LEFT_STATE};
	SDL_Rect frame;
	int camera_state;
	int offset;
};


#endif //__CAMERA_H_DEFINED__