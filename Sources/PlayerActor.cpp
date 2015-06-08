/**
 * PlayerActor.cpp
 *
 */

#include "PlayerActor.h"

PlayerActor::PlayerActor(Window *window,
						 Camera *camera,
						 PlayerController *playerController):
	playerController(playerController),
	window(window),
	camera(camera),
	texture(window, "rogue_knight_idle.png"),
	facing(RIGHT)
{

}

void PlayerActor::render()
{
	int left_border = camera->getFrame().w * 0.8;
	
		texture.render(playerController->getLocation().x,
					   playerController->getLocation().y);
	/*
	if (playerController->getLocation().x >= (camera->getFrame().x + left_border)) {
		
		texture.render(left_border,
					   playerController->getLocation().y);
	} else {
		
		texture.render(playerController->getLocation().x,
					   playerController->getLocation().y);
	}
	*/
}