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
	vector<Sound>::iterator soundIterator;

	for (soundIterator = knight->getSounds()->begin();
		soundIterator != knight->getSounds()->end(); 
		++soundIterator)
	{
		if (soundIterator->type.compare("landing") == 0) {
			sfx_land.load(soundIterator->filename);
		}

		if (soundIterator->type.compare("jump") == 0) {
			sfx_jump.load(soundIterator->filename);
		}
	}

}

void PlayerActor::updateSound()
{
	if (knight->falling == true) {
		if (knight->is_landed == true) {
			sfx_land.play(1);
			knight->falling = false;
			knight->is_jumping = false;
			knight->jumping = false;
		}
	}
	
	if (knight->jumping == true) {
		if (knight->is_jumping == false) {
			sfx_jump.play(1);
			knight->is_jumping = true;
		}
	}
}

void PlayerActor::updateAnimation()
{
	currentAnimation = knight->getAnimations(knight->IDLE);
	moves = playerController->moves;
	
	if (playerController->velocity_x != 0) {
		currentAnimation = knight->getAnimations(knight->RUN);
	}
	
	if (playerController->jumping == true) {
		currentAnimation = knight->getAnimations(knight->JUMP);
	}

	if (knight->falling == true) {
		currentAnimation = knight->getAnimations(knight->JUMP);
	}
	
	SpecialCombo *tmp_special;
	tmp_special = &(*moves).at(knight->ATTACK);

	if (playerController->attacking == true) {
		if (!playerController->in_air) {
			currentAnimation = knight->getAnimations(knight->ATTACK);
		} else {
			currentAnimation = knight->getAnimations(knight->MID_AIR_BASIC_ATTACK);
		}
		
		if (currentAnimation->getCurrentFrame() == 0 && currentAnimation->times_played > 0) {
		 	playerController->attacking = false;
			knight->getMoves()->at(knight->ATTACK).executing = false;
			knight->weapon.setHit(false);
		 	currentAnimation->times_played = 0;
			playerController->stopAttack();
		}
		
		if (currentAnimation->times_played < tmp_special->repeats) {
			Rectangle tmp_hitbox = tmp_special->hitboxes[currentAnimation->getCurrentFrame()];
			playerController->attack_hb.w = tmp_hitbox.w;
			playerController->attack_hb.h = tmp_hitbox.h;

			if (facing_direction == FACING_RIGHT) {
				playerController->attack_hb.x = playerController->location.x + tmp_hitbox.x;
			} else {
				playerController->attack_hb.x = playerController->location.x - tmp_hitbox.x - tmp_hitbox.w;
			}
				
			playerController->attack_hb.y = tmp_hitbox.y + playerController->location.y;
		}
	}

	// BLOCK

	// CROUCH
	if (playerController->crouching == true) {
		currentAnimation = knight->getAnimations(knight->CROUCH);
	}

	// DODGE
	// DOWN_THRUST
	tmp_special = &(*moves).at(knight->DOWN_THRUST);
	if (tmp_special->executing == true) {
		currentAnimation = knight->getAnimations(knight->DOWN_THRUST);

		if (currentAnimation->getCurrentFrame() == 0 && currentAnimation->times_played > 0) {
		 	tmp_special->executing = false;
			knight->getMoves()->at(knight->DOWN_THRUST).executing = false;
		 	currentAnimation->times_played = 0;
			playerController->executing_combo = false;
			playerController->stopAttack();
		}

		if (currentAnimation->times_played < tmp_special->repeats) {
			Rectangle tmp_hitbox = tmp_special->hitboxes[currentAnimation->getCurrentFrame()];
			playerController->attack_hb.w = tmp_hitbox.w;
			playerController->attack_hb.h = tmp_hitbox.h;

			if (facing_direction == FACING_RIGHT) {
				playerController->attack_hb.x = playerController->location.x + tmp_hitbox.x;
			} else {
				playerController->attack_hb.x = playerController->location.x - tmp_hitbox.x - tmp_hitbox.w;
			}
				
			playerController->attack_hb.y = tmp_hitbox.y + playerController->location.y;
		}
	}

	// HANGING
	// PUSHBACK
	// SPECIAL_I
	tmp_special = &(*moves).at(knight->SPECIAL_I);
	if (tmp_special->executing == true) {
		currentAnimation = knight->getAnimations(knight->SPECIAL_I);

		if (currentAnimation->getCurrentFrame() == 0 && currentAnimation->times_played > 0) {
		 	tmp_special->executing = false;
			knight->getMoves()->at(knight->SPECIAL_I).executing = false;
		 	currentAnimation->times_played = 0;
			playerController->executing_combo = false;
			playerController->stopAttack();
		}

		if (currentAnimation->times_played < tmp_special->repeats) {
			Rectangle tmp_hitbox = tmp_special->hitboxes[currentAnimation->getCurrentFrame()];
			playerController->attack_hb.w = tmp_hitbox.w;
			playerController->attack_hb.h = tmp_hitbox.h;

			if (facing_direction == FACING_RIGHT) {
				playerController->attack_hb.x = playerController->location.x + tmp_hitbox.x;
			} else {
				playerController->attack_hb.x = playerController->location.x - tmp_hitbox.x - tmp_hitbox.w;
			}
				
			playerController->attack_hb.y = tmp_hitbox.y + playerController->location.y;
		}
	}
	/*
	// SPECIAL_II
	if ((*moves)[knight->SPECIAL_II].executing == true) {
		currentAnimation = knight->getAnimations(knight->SPECIAL_II);

		if (currentAnimation->getCurrentFrame() == 0 && currentAnimation->times_played > 0) {
		 	(*moves)[knight->SPECIAL_II].executing = false;
		 	currentAnimation->times_played = 0;
			playerController->executing_combo = false;
		}
	}

	// SPECIAL_III
	if ((*moves)[knight->SPECIAL_III].executing == true) {
		currentAnimation = knight->getAnimations(knight->SPECIAL_III);

		if (currentAnimation->getCurrentFrame() == 0 && currentAnimation->times_played > 0) {
		 	(*moves)[knight->SPECIAL_III].executing = false;
		 	currentAnimation->times_played = 0;
			playerController->executing_combo = false;
		}
	}

	// SPECIAL_IV
	if ((*moves)[knight->SPECIAL_IV].executing == true) {
		currentAnimation = knight->getAnimations(knight->SPECIAL_IV);

		if (currentAnimation->getCurrentFrame() == 0 && currentAnimation->times_played > 0) {
		 	(*moves)[knight->SPECIAL_IV].executing = false;
		 	currentAnimation->times_played = 0;
			playerController->executing_combo = false;
		}
	}

	*/
	// THROW
	// UPPERCUT
	tmp_special = &(*moves).at(knight->UPPERCUT);
	if ((*moves)[knight->UPPERCUT].executing) {

		currentAnimation = knight->getAnimations(knight->UPPERCUT);
			
		if (currentAnimation->getCurrentFrame() == 0 && currentAnimation->times_played > 0) {
		 	playerController->attacking = false;
			knight->getMoves()->at(knight->UPPERCUT).executing = false;
			knight->weapon.setHit(false);
		 	currentAnimation->times_played = 0;
			playerController->executing_combo = false;
			playerController->stopAttack();
		}
		
		if (currentAnimation->times_played < tmp_special->repeats) {
			Rectangle tmp_hitbox = tmp_special->hitboxes[currentAnimation->getCurrentFrame()];
			playerController->attack_hb.w = tmp_hitbox.w;
			playerController->attack_hb.h = tmp_hitbox.h;

			if (facing_direction == FACING_RIGHT) {
				playerController->attack_hb.x = playerController->location.x + tmp_hitbox.x;
			} else {
				playerController->attack_hb.x = playerController->location.x - tmp_hitbox.x - tmp_hitbox.w;
			}
				
			playerController->attack_hb.y = tmp_hitbox.y + playerController->location.y;
		}

	}

	if (knight->alive) {
		currentAnimation->play(INFINITE_LOOP);
	}

	// DEATH
	if (knight->alive == false) {
		currentAnimation = knight->getAnimations(knight->DEATH);

		if (knight->out_of_bounds) {
			currentAnimation->times_played = 1;
		}

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
	bool draw_attack_hitbox =	false;

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