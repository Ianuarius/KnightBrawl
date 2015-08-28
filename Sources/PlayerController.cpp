/**
 * PlayerController.cpp
 *
 */

#include "PlayerController.h"

// XBOX 360 Mapping:
// http://wiki.gp2x.org/articles/s/d/l/SDL_Joystick_mapping.html

PlayerController::PlayerController(bool multiplayer, int player, std::vector<SDL_Point> *positions):
	multiplayer(multiplayer),
	player(player),
	positions(positions),
	in_menu(true),
	boundbox(0, 0, 0, 0), // NOTE(juha): Init due no default constructor.
	hitbox(boundbox),
	desired(boundbox),
	attack_hb(boundbox),
	menu_x(0 + player), menu_y(0)
{	
	parseMappedValues();
	in_game =			false;
	gamepad_ready =		false;
	controller_locked = true;
	AButton =			false;
	XButton =			false;
	Up =				false;
	Down =				false;
	Left =				false;
	Right =				false;
}

PlayerController::PlayerController(SDL_Point start_position, bool multiplayer, int player, Knight *knight):
	multiplayer(multiplayer),
	player(player),
	knight(knight),
	attack_w(knight->getMoves()->at(knight->ATTACK).hitboxes[0].w),
	attack_h(knight->getMoves()->at(knight->ATTACK).hitboxes[0].h),
	location(start_position),
	boundbox(location.x - 25, location.y - 50, 50, 50),
	hitbox(location.x + knight->hitbox.x, 
		   location.y + knight->hitbox.y, 
		   knight->hitbox.w, 
		   knight->hitbox.h),
	desired(hitbox),
	attack_hb(location.x - 25, location.y + 40, 0, 0),
	pushback_angle(0), pushback_power(10),
	acceleration(0.7f), stoppedThreshold(acceleration/7),
	velocity_x(0), velocity_y(0),
	targetVx(0),
	facing_direction(FACING_RIGHT),
	speed(knight->getSpeed()),
	moves(knight->getMoves()),
	combo_one_state(0)
{
	// NOTE(juha): Initialization
	attacking =			false;
	basic_attack =		false;
	controller_locked = true;
	crouching =			false;
	executing_combo =	false;
	gamepad_ready =		false;
	has_attacked =		false;
	has_jumped =		false;
	in_air =			true;
	in_menu =			false;
	jumping =			false;
	
	// NOTE(juha): Controller initialization
	AButton =			false;
	XButton =			false;
	Up =				false;
	Down =				false;
	Left =				false;
	Right =				false;

	for (int i = 0; i < SPECIAL_MOVES; ++i) {
		combo_state.push_back(0);
	}
	parseMappedValues();

	moves_amount = knight->ANIMATION_MAX;
}

void PlayerController::setGamepad(SDL_GameController *NewControllerHandle)
{
	ControllerHandle = NewControllerHandle;
	if (ControllerHandle) {
		gamepad_ready =	true;
	}
}

void PlayerController::setPlayers(int *active_players)
{
	players = active_players;
}

void PlayerController::parseMappedValues()
{
	std::string filename = "Controls.xml";
	result = controlsDocument.load_file(filename.c_str());
	SinglePlayerMappings = 
		controlsDocument.child("profile").
		find_child_by_attribute("name", "singleplayer");
	MultiPlayerMappings = 
		controlsDocument.child("profile").
		find_child_by_attribute("controller", std::to_string(player).c_str());
	
	if (!result) {
		printf("Failed to load controls file: %s.\n", filename.c_str());
		return;
	}
	


	// NOTE(juha): Parsing the inputs from the mapped values.
	if (!multiplayer) {
		key_left = fieldTypeParser.ParseSomeEnum(SinglePlayerMappings.
			find_child_by_attribute("name", "move_left").attribute("keyboard").value());
		key_right = fieldTypeParser.ParseSomeEnum(SinglePlayerMappings.
			find_child_by_attribute("name", "move_right").attribute("keyboard").value());
		key_down = fieldTypeParser.ParseSomeEnum(SinglePlayerMappings.
			find_child_by_attribute("name", "crouch").attribute("keyboard").value());
		key_up = fieldTypeParser.ParseSomeEnum(SinglePlayerMappings.
			find_child_by_attribute("name", "up").attribute("keyboard").value());
		key_jump = fieldTypeParser.ParseSomeEnum(SinglePlayerMappings.
			find_child_by_attribute("name", "jump").attribute("keyboard").value());
		key_action = fieldTypeParser.ParseSomeEnum(SinglePlayerMappings.
			find_child_by_attribute("name", "action").attribute("keyboard").value());
		key_menu = fieldTypeParser.ParseSomeEnum(SinglePlayerMappings.
			find_child_by_attribute("name", "menu").attribute("keyboard").value());
	} else {
		key_left = fieldTypeParser.ParseSomeEnum(MultiPlayerMappings.
			find_child_by_attribute("name", "move_left").attribute("keyboard").value());
		key_right = fieldTypeParser.ParseSomeEnum(MultiPlayerMappings.
			find_child_by_attribute("name", "move_right").attribute("keyboard").value());
		key_down = fieldTypeParser.ParseSomeEnum(MultiPlayerMappings.
			find_child_by_attribute("name", "crouch").attribute("keyboard").value());
		key_up = fieldTypeParser.ParseSomeEnum(MultiPlayerMappings.
			find_child_by_attribute("name", "up").attribute("keyboard").value());
		key_jump = fieldTypeParser.ParseSomeEnum(MultiPlayerMappings.
			find_child_by_attribute("name", "jump").attribute("keyboard").value());
		key_action = fieldTypeParser.ParseSomeEnum(MultiPlayerMappings.
			find_child_by_attribute("name", "action").attribute("keyboard").value());
		key_menu = fieldTypeParser.ParseSomeEnum(MultiPlayerMappings.
			find_child_by_attribute("name", "menu").attribute("keyboard").value());
	}
}

void PlayerController::update()
{
	if (gamepad_ready) {
		AButton =	SDL_GameControllerGetButton(ControllerHandle, SDL_CONTROLLER_BUTTON_A);
		XButton =	SDL_GameControllerGetButton(ControllerHandle, SDL_CONTROLLER_BUTTON_X);
		Up =		SDL_GameControllerGetButton(ControllerHandle, SDL_CONTROLLER_BUTTON_DPAD_UP);
		Down =		SDL_GameControllerGetButton(ControllerHandle, SDL_CONTROLLER_BUTTON_DPAD_DOWN);
		Left =		SDL_GameControllerGetButton(ControllerHandle, SDL_CONTROLLER_BUTTON_DPAD_LEFT);
		Right =		SDL_GameControllerGetButton(ControllerHandle, SDL_CONTROLLER_BUTTON_DPAD_RIGHT);
	
		if (!AButton)	AButton_up =	true;
		if (!XButton)	XButton_up =	true;
		if (!Up)		Up_up =			true;
		if (!Down)		Down_up =		true;
		if (!Left)		Left_up =		true;
		if (!Right)		Right_up =		true;
	}

	if (!in_menu) {
		// NOTE(juha): Gravitational stuff
		velocity_y += GRAVITY * (16.f / 1000);
		desired.y += (int)velocity_y;
		crouching = false;
	
		if (velocity_y >= 1) {
			in_air = true;
			knight->falling = true;
			knight->is_landed = false;
		}
	
		if (velocity_y >= 7) {
			velocity_y = 7;
		}
	
		if (!in_air) {
			jumping = false;
		}
	}
	
	if (in_menu) {
		if (player == 3 && *players == 2) {
			controller_locked = true;
		} else {
			controller_locked = false;
		}
	} else {
		controller_locked = false;
	}
	
	// NOTE(juha): Reading the inputs for single player and multiplayer.
	int tmp_input = 9999;
	
	// MOVE LEFT
	if (playerInput.keyState(key_left) || (Left && !controller_locked)) {
		if ((playerInput.isKeyPressed(key_left) && playerInput.isKeyDown(key_left)) || 
			(Left_up && !controller_locked && gamepad_ready)) {
			if (!in_menu) {
				if (facing_direction == FACING_LEFT) {
					tmp_input = knight->FORWARD;
				} else {
					tmp_input = knight->BACKWARD;
				}
			}
			menu_x -= 1;
		}
		left();
		Left_up = false;
	}
		
	// MOVE RIGHT
	if (playerInput.keyState(key_right) || (Right && !controller_locked)) {
		if ((playerInput.isKeyPressed(key_right) && playerInput.isKeyDown(key_right)) || 
			(Right_up && !controller_locked && gamepad_ready)) {
			if (!in_menu) {
				if (facing_direction == FACING_RIGHT) {
					tmp_input = knight->FORWARD;
				} else {
					tmp_input = knight->BACKWARD;
				}
			}
			menu_x += 1;
		}
		right();
		Right_up = false;
	}
		
	// DOWN BUTTON
	if (playerInput.keyState(key_down) || (Down && !controller_locked)) {
		if ((playerInput.isKeyPressed(key_down) && playerInput.isKeyDown(key_down)) || 
			(Down_up && !controller_locked && gamepad_ready)) {
			if (!in_menu) {
				tmp_input = knight->DOWN;
			}
			menu_y += 1;
		}
		crouch();
		Down_up = false;
	}
		
	// UP BUTTON
	if (playerInput.keyState(key_up) || (Up && !controller_locked)) {
		if ((playerInput.isKeyPressed(key_up) && playerInput.isKeyDown(key_up)) || 
			(Up_up && !controller_locked && gamepad_ready)) {
			if (!in_menu) {
				tmp_input = knight->UP;
			}
			menu_y -= 1;
		}
		up();
		Up_up = false;
	}
	
	// JUMP BUTTON
	if (playerInput.keyState(key_jump) || (AButton && !controller_locked)) {
		if ((playerInput.isKeyPressed(key_jump) && playerInput.isKeyDown(key_jump) && !in_menu) || 
			(AButton_up && !controller_locked && gamepad_ready)) {
			tmp_input = knight->JUMP;
		}
		if (!has_jumped) {
			jump();
			AButton_up = false;
			has_jumped = true;
		}
	}
	if (playerInput.isKeyUp(key_jump)) {
		has_jumped = false;
	}
		
	// ACTION BUTTON
	if (playerInput.keyState(key_action) || (XButton && !controller_locked)) {
		if ((playerInput.isKeyPressed(key_action) && playerInput.isKeyDown(key_action)) || 
			(XButton_up && !controller_locked && gamepad_ready)) {
			if (!in_menu) {
				tmp_input = knight->ACTION;
			} else {
				if (in_game == false) {
					in_game = true;
					*players += 1;
				}
			}
		}
		if (!has_attacked) {
			basic_attack = true;
			XButton_up = false;
			has_attacked = true;
		}
	}
		
	if (playerInput.isKeyUp(key_action)) {
		has_attacked = false;
	}
	// MENU BUTTON
	if (playerInput.keyState(key_menu)) {
	}
	
	
	if (!in_menu) {

		// NOTE(juha): respawn timer
		if (knight->alive == false) {
			if (deathTimer.isStarted() == false) {
				deathTimer.start();
			}
			if (deathTimer.getTicks() >= RESPAWN_TIME) {
				knight->respawn();
				deathTimer.stop();
			}
		}

		// NOTE(juha): Goes through all the special combos.
		for (int i = 0; i < moves_amount; ++i) {
			(*moves)[i].tmp_input = tmp_input;
			
			bool continue_execution = false;
			if ((*moves)[i].keys.size() > 0) {
				
				if ((*moves)[i].keys.size() > 1) {
					// NOTE(juha): If the player just turned, then the first
					// key can be counted as forward instead of backward.
					if ((*moves)[i].keys[0].keycode == knight->FORWARD &&
						(*moves)[i].tmp_input == knight->BACKWARD &&
						(*moves)[i].state == 0) {
						(*moves)[i].tmp_input = knight->FORWARD;
					}

					// NOTE(juha): tmp_input 9999 means that no keys were pressed.
					// NOTE(juha): If a right key was pressed, move combos that had that key
					// to the next state. If nothing was pressed, do nothing. If the key
					// wasn't a part of the combo, then return it to the beginning state.
					if ((*moves)[i].keys[(*moves)[i].state].keycode == (*moves)[i].tmp_input &&
						(*moves)[i].executing == false) {
						(*moves)[i].state++;
					} else if ((*moves)[i].tmp_input == 9999) {
						// Do nothing.
					} else {
						(*moves)[i].state = 0;
					}
				

					for (int j = 0; j < (*moves)[i].keys.size(); j++) {
						if ((*moves)[i].keys[j].pressed == true) {

							if ((*moves)[i].keys[j].keycode == knight->FORWARD &&
								facing_direction == FACING_LEFT) {
								
								if (playerInput.isKeyPressed(key_left)) {
									continue_execution = true;
								}
							} else if ((*moves)[i].keys[j].keycode == knight->BACKWARD &&
								facing_direction == FACING_LEFT) {
								
								if (playerInput.isKeyPressed(key_right)) {
									continue_execution = true;
								}
							}  else if ((*moves)[i].keys[j].keycode == knight->FORWARD &&
								facing_direction == FACING_RIGHT) {
								
								if (playerInput.isKeyPressed(key_right)) {
									continue_execution = true;
								}
							}  else if ((*moves)[i].keys[j].keycode == knight->BACKWARD &&
								facing_direction == FACING_RIGHT) {
								
								if (playerInput.isKeyPressed(key_left)) {
									continue_execution = true;
								}
							} else if ((*moves)[i].keys[j].keycode == knight->JUMP) {
								
								if (playerInput.isKeyPressed(key_jump)) {
									continue_execution = true;
								}
							} else if ((*moves)[i].keys[j].keycode == knight->DOWN) {
								
								if (playerInput.isKeyPressed(key_down)) {
									continue_execution = true;
								}
							} else if ((*moves)[i].keys[j].keycode == knight->UP) {
								
								if (playerInput.isKeyPressed(key_up)) {
									continue_execution = true;
								}
							} else if ((*moves)[i].keys[j].keycode == knight->ACTION) {
								
								if (playerInput.isKeyPressed(key_action)) {
									continue_execution = true;
								}
							}

						} else {
							continue_execution = true;
						}
						
						if ((*moves)[i].keys[j].in_air && !in_air) {
							continue_execution = false;
						}
					}
					
					if ((*moves)[i].in_ground && in_air) {
						continue_execution = false;
					}
					

					if (continue_execution == false) {
						(*moves)[i].state = 0;
					}
					
					int combopower = 5;
					if ((*moves)[i].keys.size() == (*moves)[i].state &&
						continue_execution == true) {
						if ((*moves)[i].disabled == false && knight->getSpecialPower() > combopower) {
							(*moves)[i].executing = true;
							executing_combo = true;
							knight->executeCombo(combopower);
							printf("executing\n");
						}
						(*moves)[i].state = 0;
					}
				}
			}
		}

		if (basic_attack) {
			basicAttack();
			basic_attack = false;
		}
	
		if (jumping) {
			jump();
		}
	
		// NOTE(juha): Calculate horizontal movement.
		velocity_x += ((targetVx - velocity_x) * acceleration) * 0.16667f;
	
		if (fabs(velocity_x) > speed) {
			velocity_x = (velocity_x > 0) ? (speed+1) : -speed;
		}

		if (fabs(velocity_x) < stoppedThreshold) {
			velocity_x = 0;
		}

		if (facing_direction == FACING_RIGHT) {
			desired.x += (int)ceilf(fabs(velocity_x));
		} else {
			desired.x -= (int)ceilf(fabs(velocity_x));
		}

		int deduct = 0;

		attack_hb.y = desired.y + 8;

		targetVx = 0;

		if (movements.size() > 0) {
			for (int i = 0; i < movements.size(); i++) {
				if (movements.at(i).executing &&
					movements.at(i).distance_travelled < movements.at(i).range) {
				
					double new_x = movements.at(0).speed * cos(movements.at(i).angle * PI / 180);
					double new_y = movements.at(0).speed * sin(movements.at(i).angle * PI / 180);
	
					if (facing_direction == 1) {
						desired.x += new_x;
					} else {
						desired.x -= new_x;
					}

					desired.y -= new_y;
					movements.at(i).distance_travelled += movements.at(i).speed;
				} else {
					movements.erase(movements.begin() + i);
				}
			}
		}
	}
}

void PlayerController::updateInput()
{
	playerInput.update();

}

void PlayerController::stopAttack()
{
	pushback_angle = 0;
	pushback_power = 10;
	attack_hb.w = 0;
	attack_hb.h = 0;
}

void PlayerController::knockBack(int attack_direction, int attack_angle, int attack_power)
{
	double new_x = attack_power * cos(attack_angle * PI / 180);
	double new_y = attack_power * sin(attack_angle * PI / 180);
	
	if (attack_direction == FACING_RIGHT) {
		desired.x += new_x;
	} else {
		desired.x -= new_x;
	}
	
	desired.y -= new_y;

	if (attack_direction != facing_direction) {
		velocity_x = 0;
	}
}

int PlayerController::getDirection()
{
	return facing_direction;
}

void PlayerController::commitMovement()
{
	location.x = desired.x + hitbox.w/2;
	location.y = desired.y + hitbox.h;

	boundbox.x = location.x - 25;
	boundbox.y = location.y - 50;
	hitbox.x = location.x + knight->hitbox.x;
	hitbox.y = location.y + knight->hitbox.y;
}

Knight *PlayerController::getKnight()
{
	return knight;
}

void PlayerController::move(int x, int y)
{
	desired.x += x;
	desired.y += y;
}

// NOTE(juha): Collects all the action methods into one
// Changes the PlayerController STATE instead of it having many different bools
/*
void PlayerController::doAction(int action)
{
	switch (action)
	{
	default:
		break;
	}
}
*/

void PlayerController::left()
{
	if (!in_menu) {
		targetVx = -(speed * acceleration);
		facing_direction = FACING_LEFT;
	} else {
	}
}

void PlayerController::right()
{
	if (!in_menu) {
		targetVx = (speed * acceleration);
		facing_direction = FACING_RIGHT;
	} else {
	}
}

void PlayerController::up()
{
	if (!in_menu) {
	} else {
	}
}

void PlayerController::jump()
{
	if (!in_menu) {
		if (in_air == false) {
			velocity_y -= knight->getJump() + 0.4;
			in_air = true;
			jumping = true;
			knight->jumping = true;
			knight->landing = false;
		}
	} else {

	}
}

void PlayerController::basicAttack()
{
	if (!in_menu && !executing_combo) {
		attacking = true;
		knight->getMoves()->at(knight->ATTACK).executing = true;
		basic_attack = false;
	} else {

	}
}

void PlayerController::crouch()
{
	if (!in_menu) {
		if (!jumping && !in_air) {
			crouching = true;
			velocity_x = 0;
			targetVx = 0;
		}
	} else {
	}
}

// BLOCK
// DEATH
// DODGE
// DOWN_THRUST
// HANGING
// MID_AIR_BASIC_ATTACK
// PUSHBACK

// THROW
// UPPERCUT
