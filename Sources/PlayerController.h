/**
 * PlayerController.h
 *
 */

#ifndef __PLAYERCONTROLLER_H_DEFINED__
#define __PLAYERCONTROLLER_H_DEFINED__

#include "Input.h"
#include "SDL.h"

class PlayerController
{
public:
	PlayerController();
	SDL_Point getLocation();
	void update();
	
private:
	SDL_Point location;
};

#endif //__PLAYERCONTROLLER_H_DEFINED__