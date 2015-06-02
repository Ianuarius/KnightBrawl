
/**
 * PlayerActor.h
 *
 */

#ifndef __PLAYERACTOR_H_DEFINED__
#define __PLAYERACTOR_H_DEFINED__

#include "SDL.h"
#include "Texture.h"
#include "PlayerController.h"
#include "Camera.h"

class PlayerActor
{
public:
	PlayerActor(Window *window,
			    Camera *camera,
				PlayerController *playerController);
	void render();
	int facing;
	float facing_position;
	
private:
	Window *window;
	PlayerController *playerController;
	Camera *camera;
	Texture texture;
};

#endif //__PLAYERACTOR_H_DEFINED__