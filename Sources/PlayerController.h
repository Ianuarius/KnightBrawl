/**
 * PlayerController.h
 *
 */

#ifndef __PLAYERCONTROLLER_H_DEFINED__
#define __PLAYERCONTROLLER_H_DEFINED__

#include "Input.h"
#include "SDL.h"

enum Direction {RIGHT, LEFT};

class PlayerController
{
public:
	PlayerController(SDL_Point start_position);
	SDL_Point getLocation();
	int getDirection();
	
	void update();
	
private:
	SDL_Point location;
	int facing;

};

#endif //__PLAYERCONTROLLER_H_DEFINED__