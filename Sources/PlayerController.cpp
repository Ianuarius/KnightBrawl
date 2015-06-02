/**
 * PlayerController.cpp
 *
 */

#include "PlayerController.h"

PlayerController::PlayerController(SDL_Point start_position):
	location(start_position)
{

}

void PlayerController::update()
{
	if (Input::keyState(SDL_SCANCODE_D)) {
		location.x += 10;
		facing = RIGHT;
	}
		
	if (Input::keyState(SDL_SCANCODE_A)) {
		location.x -= 10;
		facing = LEFT;
	}
		
	if (Input::keyState(SDL_SCANCODE_S)) {
		location.y += 10;
	}
		
	if (Input::keyState(SDL_SCANCODE_W)) {
		location.y -= 10;
	}
}

SDL_Point PlayerController::getLocation()
{
	return location;
}

int PlayerController::getDirection()
{
	return facing;
}