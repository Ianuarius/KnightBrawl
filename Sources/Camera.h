/**
 * Camera.h
 *
 */

#ifndef __CAMERA_H_DEFINED__
#define __CAMERA_H_DEFINED__

#include "SDL.h"

class Camera
{

public:
	Camera(int width, int height);
	void update(int x, int y);
	SDL_Rect frame;

private:

};


#endif //__CAMERA_H_DEFINED__