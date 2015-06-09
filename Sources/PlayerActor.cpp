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
	/*
	int facing_l = camera->getFrame().w * 0.4f;
	int facing_r = camera->getFrame().w * 0.6f;
	*/
	
	int camera_middle_x = camera->getFrame().w / 2;

	texture.render(camera->getFrame().x + camera_middle_x,
				   camera->getFrame().y);
	

	/*
	if (camera->getState() == RIGHT) {
		
		if (playerController->getLocation().x >=
		   (camera->getFrame().x + facing_l)) {
		
			texture.render(facing_l,
						   playerController->getLocation().y);

		} else {
			
			texture.render(playerController->getLocation().x - camera->getFrame().x,
						   playerController->getLocation().y);
		}

	} else if (camera->getState() == LEFT) {

		if (playerController->getLocation().x <=
		   (camera->getFrame().x + facing_r)) {

			  texture.render(facing_r,
							 playerController->getLocation().y);

		} else {
			
			texture.render(playerController->getLocation().x - camera->getFrame().x,
						   playerController->getLocation().y);
		}
	}*/
	
}

/* vain v‰h‰n tilaa liikkua, joten pit‰‰ hakea se kohta, miss‰ voi liikkua
   eli haetaan kamerasta se kohta, miss‰ pelaaja on, eli se pieni tila, miss‰ voi
   liikkua vapaasti ja sitten v‰hennet‰‰n sit‰ sit‰ mukaa kun pelaaja liikkuu
*/