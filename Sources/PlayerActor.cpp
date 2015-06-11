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
	int camera_middle_x = camera->getFrame().w / 2;

	texture.render(camera->getFrame().x + camera_middle_x,
				   camera->getFrame().y);
}