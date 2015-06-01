
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
				PlayerController *playerController, 
				SDL_Point start_position);
	void render();
	SDL_Point position;
	int facing;
	float facing_position;
	
private:
	Window *window;
	PlayerController *playerController;
	Camera *camera;
	Texture texture;
	enum direction {RIGHT, LEFT};
};

#endif //__PLAYERACTOR_H_DEFINED__