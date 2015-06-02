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
	texture.render((camera->getFrame().w / 2) + camera->getOffset() -25,
				   (camera->getFrame().h) - 80);
}