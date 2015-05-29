
/**
 * PlayerActor.h
 *
 */

#ifndef __PLAYERACTOR_H_DEFINED__
#define __PLAYERACTOR_H_DEFINED__

#include "SDL.h"

class PlayerActor
{
public:
	PlayerActor(SDL_Point start_position);
	SDL_Point position;
	
private:
};

#endif //__PLAYERACTOR_H_DEFINED__