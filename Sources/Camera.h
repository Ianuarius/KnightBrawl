/**
 * Camera.h
 *
 */

#ifndef __CAMERA_H_DEFINED__
#define __CAMERA_H_DEFINED__

#define FACING_L 0.4f
#define FACING_R 0.6f
#define PAN_TR_L 0.3f
#define PAN_TR_R 0.7f
#define LERP 0.1f

#include "SDL.h"
#include "PlayerActor.h"

class Camera
{

public:
	Camera(int width, int height);
	void lock(PlayerActor *player);
	void update();
	SDL_Rect frame;

private:
	enum Direction {RIGHT, LEFT};
	PlayerActor *lockedPlayer;
	int facing;
};


#endif //__CAMERA_H_DEFINED__