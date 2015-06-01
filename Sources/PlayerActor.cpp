/**
 * PlayerActor.cpp
 *
 */

#include "PlayerActor.h"

PlayerActor::PlayerActor(Window *window, 
						 Camera *camera, 
						 PlayerController *playerController, 
						 SDL_Point start_position):
	position(start_position),
	window(window),
	texture(window, "rogue_knight_idle.png"),
	facing(RIGHT)
{

}

void PlayerActor::render()
{
	texture.render(position.x, position.y);
}