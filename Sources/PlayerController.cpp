/**
 * PlayerController.cpp
 *
 */

#include "PlayerController.h"

PlayerController::PlayerController()
{

}

void PlayerController::update()
{
	if (Input::keyState(SDL_SCANCODE_D)) {
		location.x += 10;
		//player->facing_position = 0.4f;
	}
		
	if (Input::keyState(SDL_SCANCODE_A)) {
		location.x -= 10;
		//player->facing_position = 0.6f;
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