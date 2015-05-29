/**
 * PlayerController.cpp
 *
 */

#include "PlayerController.h"

PlayerController::PlayerController(PlayerActor *player):
	player(player)
{

}

void PlayerController::update()
{
	if (Input::keyState(SDL_SCANCODE_D)) {
		player->position.x += 10;
	}
		
	if (Input::keyState(SDL_SCANCODE_A)) {
		player->position.x -= 10;
	}
		
	if (Input::keyState(SDL_SCANCODE_S)) {
		player->position.y += 10;
	}
		
	if (Input::keyState(SDL_SCANCODE_W)) {
		player->position.y -= 10;
	}
}