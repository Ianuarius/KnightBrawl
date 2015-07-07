/**
 * PlayerActor.cpp
 *
 */

#include "PlayerActor.h"

PlayerActor::PlayerActor(Window *window,
						 Camera *camera,
						 PlayerController *playerController,
						 Knight *knight):
	playerController(playerController),
	window(window),
	camera(camera),
	knight(knight),
	facing_direction(FACING_RIGHT),
	special_combos(playerController->special_combos)
{
}

void PlayerActor::updateAnimation()
{
	currentAnimation = knight->getAnimations(knight->IDLE);
	
	if (playerController->velocity_x != 0) {
		currentAnimation = knight->getAnimations(knight->RUN);
	}
	
	if (playerController->jumping == true) {
		currentAnimation = knight->getAnimations(knight->JUMP);
	}

	if (playerController->attacking == true) {
		currentAnimation = knight->getAnimations(knight->ATTACK);
		
		if (currentAnimation->getCurrentFrame() == 0 && currentAnimation->times_played > 0) {
		 	playerController->attacking = false;
			knight->hit = false;
		 	currentAnimation->times_played = 0;
		}

		if (currentAnimation->getCurrentFrame() > 0) {
			playerController->attack_hb.w = 36;
			playerController->attack_hb.h = 10;
		} else {
			playerController->attack_hb.w = 0;
			playerController->attack_hb.h = 0;
		}
	}
	// BLOCK

	// CROUCH
	if (playerController->crouching == true) {
		currentAnimation = knight->getAnimations(knight->CROUCH);
	}

	// DODGE
	// DOWN_THRUST
	// HANGING
	// MID_AIR_BASIC_ATTACK
	// PUSHBACK

	// SPECIAL_I
	if ((*special_combos)[0].executing == true) {
		currentAnimation = knight->getAnimations(knight->SPECIAL_I);

		if (currentAnimation->getCurrentFrame() == 0 && currentAnimation->times_played > 0) {
		 	(*special_combos)[0].executing = false;
		 	currentAnimation->times_played = 0;
		}
	}
	// SPECIAL_II

	
	if ((*special_combos)[1].executing == true) {
		currentAnimation = knight->getAnimations(knight->SPECIAL_II);

		if (currentAnimation->getCurrentFrame() == 0 && currentAnimation->times_played > 0) {
		 	(*special_combos)[1].executing = false;
		 	currentAnimation->times_played = 0;
		}
	}
	// SPECIAL_III
	if ((*special_combos)[2].executing == true) {
		currentAnimation = knight->getAnimations(knight->SPECIAL_III);

		if (currentAnimation->getCurrentFrame() == 0 && currentAnimation->times_played > 0) {
		 	(*special_combos)[2].executing = false;
		 	currentAnimation->times_played = 0;
		}
	}
	// SPECIAL_IV
	if ((*special_combos)[3].executing == true) {
		currentAnimation = knight->getAnimations(knight->SPECIAL_IV);

		if (currentAnimation->getCurrentFrame() == 0 && currentAnimation->times_played > 0) {
		 	(*special_combos)[3].executing = false;
		 	currentAnimation->times_played = 0;
		}
	}
	// THROW
	// UPPERCUT
	if (knight->alive) {
		currentAnimation->play(INFINITE_LOOP);
	}

	
	// DEATH
	if (knight->alive == false) {
		currentAnimation = knight->getAnimations(knight->DEATH);

		if (currentAnimation->times_played == 0) {
			currentAnimation->play(1);
		}
	}

}

void PlayerActor::render()
{	
	facing_direction = playerController->getDirection();

	if (facing_direction == FACING_LEFT) {
		currentAnimation->flip = true;
	} else {
		currentAnimation->flip = false;
	}

	bool draw_boundbox =		false;
	bool draw_attack_hitbox =	true;

	int camera_middle_x = camera->getFrame().w / 2;

	if (draw_attack_hitbox) {
		window->drawRect(playerController->attack_hb.x - camera->getFrame().x,
						 playerController->attack_hb.y - camera->getFrame().y,
						 playerController->attack_hb.w,
						 playerController->attack_hb.h,
						 Color("red"));
	}

	if (draw_boundbox) {
		window->drawRect(playerController->boundbox.x - camera->getFrame().x,
						 playerController->boundbox.y - camera->getFrame().y,
						 playerController->boundbox.w,
						 playerController->boundbox.h,
						 Color("red"));
	}
	
	if (draw_boundbox) {
		window->drawRect(playerController->hitbox.x - camera->getFrame().x,
						 playerController->hitbox.y - camera->getFrame().y,
						 playerController->hitbox.w,
						 playerController->hitbox.h,
						 Color("blue"));
	}

	currentAnimation->render(playerController->location.x - playerController->boundbox.w / 2 - camera->getFrame().x, 
							 playerController->location.y - playerController->boundbox.h - camera->getFrame().y);
}