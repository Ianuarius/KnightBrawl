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
}

PlayerController::PlayerController(SDL_Point start_position, bool multiplayer, int player, Knight *knight):
	multiplayer(multiplayer),
	knight(knight),
	in_menu(false),
	executing_combo(false),
	player(player),
	location(start_position),
	boundbox(location.x - 25, location.y - 50, 50, 50),
	hitbox(location.x + knight->hitbox.x, 
		   location.y + knight->hitbox.y, 
		   knight->hitbox.w, 
		   knight->hitbox.h),
	desired(hitbox),
	attack_hb(desired.x -25, desired.y + 40, 0, 0),
	acceleration(0.7f), stoppedThreshold(acceleration/7),
	velocity_x(0), velocity_y(0),
	targetVx(0),
	facing_direction(FACING_RIGHT),
	speed(knight->getSpeed()),
	moves(knight->getMoves()),
	combo_one_state(0)
{
	// NOTE(juha): Initialization
	in_air =			true;
	jumping =			false;
	crouching =			false;
	attacking =			false;
	basic_attack =		false;

	for (int i = 0; i < SPECIAL_MOVES; ++i) {
		combo_state.push_back(0);
	}
	parseMappedValues();

	moves_amount = knight->ANIMATION_MAX;
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
	if (!in_menu) {
		// NOTE(juha): Gravitational stuff
		velocity_y += GRAVITY * (16.f / 1000);
		desired.y += (int)velocity_y;
		crouching = false;
	
		if (velocity_y >= 3) {
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

	// NOTE(juha): Reading the inputs for single player and multiplayer.
	int tmp_input = 9999;

	// MOVE LEFT
	if (playerInput.keyState(key_left)) {
		if (playerInput.isKeyPressed(key_left) && playerInput.isKeyDown(key_left)) {
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
	}
		
	// MOVE RIGHT
	if (playerInput.keyState(key_right)) {
		if (playerInput.isKeyPressed(key_right) && playerInput.isKeyDown(key_right)) {
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
	}
		
	// DOWN BUTTON
	if (playerInput.keyState(key_down)) {
		if (playerInput.isKeyPressed(key_down) && playerInput.isKeyDown(key_down)) {
			if (!in_menu) {
				tmp_input = knight->DOWN;
			}
			menu_y += 1;
		}
		crouch();
	}
		
	// UP BUTTON
	if (playerInput.keyState(key_up)) {
		if (playerInput.isKeyPressed(key_up) && playerInput.isKeyDown(key_up)) {
			if (!in_menu) {
				tmp_input = knight->UP;
			}
			menu_y -= 1;
		}
		up();
	}
	
	// JUMP BUTTON
	if (playerInput.keyState(key_jump)) {
		if (playerInput.isKeyPressed(key_jump) && playerInput.isKeyDown(key_jump) && !in_menu) {
			tmp_input = knight->JUMP;
		}
		jump();
	}
		
	// ACTION BUTTON
	if (playerInput.keyState(key_action)) {
		if (playerInput.isKeyPressed(key_action) && playerInput.isKeyDown(key_action) && !in_menu) {
			tmp_input = knight->ACTION;
		}
		basic_attack = true;
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
					}
					

					if (continue_execution == false) {
						(*moves)[i].state = 0;
					}

					if ((*moves)[i].keys.size() == (*moves)[i].state &&
						continue_execution == true) {
						if ((*moves)[i].disabled == false) {
							(*moves)[i].executing = true;
							executing_combo = true;
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

		int deduct = 23 - (36 / 2);

		if (facing_direction == FACING_RIGHT) {
			attack_hb.x = desired.x + deduct;
		} else {
			attack_hb.x = desired.x - 20 + deduct;
		}

		attack_hb.y = desired.y + 8;

		targetVx = 0;
	}
}

void PlayerController::updateInput()
{
	playerInput.update();

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

// NOTE(juha): Collects all the action methods into one
// Changes the PlayerController STATE instead of it having many different bools
void PlayerController::doAction(int action)
{
	switch (action)
	{
	default:
		break;
	}
}

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
			velocity_y -= knight->getJump();
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
