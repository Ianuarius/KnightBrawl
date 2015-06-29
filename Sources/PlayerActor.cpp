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
	facing_direction(FACING_RIGHT)
{
}

void PlayerActor::updateAnimation()
{
	// IDLE
	currentAnimation = knight->getAnimations(knight->IDLE);
	
	// RUN
	if (playerController->velocity_x != 0) {
		currentAnimation = knight->getAnimations(knight->RUN);
	}
	
	// JUMP
	if (playerController->jumping == true) {
		currentAnimation = knight->getAnimations(knight->JUMP);
	}

	// ATTACK
	// BLOCK

	// CROUCH
	if (playerController->crouching == true) {
		currentAnimation = knight->getAnimations(knight->CROUCH);
	}

	// DEATH
	// DODGE
	// DOWN_THRUST
	// HANGING
	// MID_AIR_BASIC_ATTACK
	// PUSHBACK

	// SPECIAL_I
	if (playerController->in_special_one == true) {
		currentAnimation = knight->getAnimations(knight->SPECIAL_I);

		if (currentAnimation->getCurrentFrame() == 0 && currentAnimation->times_played > 0) {
		 	playerController->in_special_one = false;
		 	currentAnimation->times_played = 0;
		}
	}
	// SPECIAL_II
	if (playerController->in_special_two == true) {
		currentAnimation = knight->getAnimations(knight->SPECIAL_II);

		if (currentAnimation->getCurrentFrame() == 0 && currentAnimation->times_played > 0) {
		 	playerController->in_special_two = false;
		 	currentAnimation->times_played = 0;
		}
	}
	// SPECIAL_III
	if (playerController->in_special_three == true) {
		currentAnimation = knight->getAnimations(knight->SPECIAL_III);

		if (currentAnimation->getCurrentFrame() == 0 && currentAnimation->times_played > 0) {
		 	playerController->in_special_three = false;
		 	currentAnimation->times_played = 0;
		}
	}
	// SPECIAL_IV
	if (playerController->in_special_four == true) {
		currentAnimation = knight->getAnimations(knight->SPECIAL_IV);

		if (currentAnimation->getCurrentFrame() == 0 && currentAnimation->times_played > 0) {
		 	playerController->in_special_four = false;
		 	currentAnimation->times_played = 0;
		}
	}
	// THROW
	// UPPERCUT
	
	currentAnimation->play(INFINITE_LOOP);
}

void PlayerActor::render()
{	
	facing_direction = playerController->getDirection();

	if (facing_direction == FACING_LEFT) {
		currentAnimation->flip = true;
	} else {
		currentAnimation->flip = false;
	}

	bool draw_boundbox = false;

	int camera_middle_x = camera->getFrame().w / 2;

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