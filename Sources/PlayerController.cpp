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

/*
Cases where the player can't move up in the selection screen:
	They're in the top row
	If there's a player on the row above, try to move past them
	If there's a player on the top row that is above, move to their side
	Same with bottom


*/


PlayerController::PlayerController(SDL_Point start_position, bool multiplayer, int player, Knight *knight):
	multiplayer(multiplayer),
	knight(knight),
	in_menu(false),
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
	sp_left = fieldTypeParser.ParseSomeEnum(SinglePlayerMappings.
		find_child_by_attribute("name", "move_left").attribute("keyboard").value());
	sp_right = fieldTypeParser.ParseSomeEnum(SinglePlayerMappings.
		find_child_by_attribute("name", "move_right").attribute("keyboard").value());
	sp_down = fieldTypeParser.ParseSomeEnum(SinglePlayerMappings.
		find_child_by_attribute("name", "crouch").attribute("keyboard").value());
	sp_up = fieldTypeParser.ParseSomeEnum(SinglePlayerMappings.
		find_child_by_attribute("name", "up").attribute("keyboard").value());
	sp_jump = fieldTypeParser.ParseSomeEnum(SinglePlayerMappings.
		find_child_by_attribute("name", "jump").attribute("keyboard").value());
	sp_action = fieldTypeParser.ParseSomeEnum(SinglePlayerMappings.
		find_child_by_attribute("name", "action").attribute("keyboard").value());
	sp_menu = fieldTypeParser.ParseSomeEnum(SinglePlayerMappings.
		find_child_by_attribute("name", "menu").attribute("keyboard").value());

	mp_left = fieldTypeParser.ParseSomeEnum(MultiPlayerMappings.
		find_child_by_attribute("name", "move_left").attribute("keyboard").value());
	mp_right = fieldTypeParser.ParseSomeEnum(MultiPlayerMappings.
		find_child_by_attribute("name", "move_right").attribute("keyboard").value());
	mp_down = fieldTypeParser.ParseSomeEnum(MultiPlayerMappings.
		find_child_by_attribute("name", "crouch").attribute("keyboard").value());
	mp_up = fieldTypeParser.ParseSomeEnum(MultiPlayerMappings.
		find_child_by_attribute("name", "up").attribute("keyboard").value());
	mp_jump = fieldTypeParser.ParseSomeEnum(MultiPlayerMappings.
		find_child_by_attribute("name", "jump").attribute("keyboard").value());
	mp_action = fieldTypeParser.ParseSomeEnum(MultiPlayerMappings.
		find_child_by_attribute("name", "action").attribute("keyboard").value());
	mp_menu = fieldTypeParser.ParseSomeEnum(MultiPlayerMappings.
		find_child_by_attribute("name", "menu").attribute("keyboard").value());
}

void PlayerController::update()
{
	// NOTE(juha): Gravitational stuff
	velocity_y += GRAVITY * (16.f / 1000);
	desired.y += (int)velocity_y;
	crouching = false;

	if (velocity_y >= 7) {
		velocity_y = 7;
	}
	
	if (!in_air) {
		jumping = false;
	}


	// NOTE(juha): Reading the inputs for single player and multiplayer.
	int tmp_input = 9999;

	if (!multiplayer) { // SINGLE PLAYER MAPPINGS
		// MOVE LEFT
		if (playerInput.keyState(sp_left)) {
			if (playerInput.isKeyPressed(sp_left) && playerInput.isKeyDown(sp_left)) {
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
		if (playerInput.keyState(sp_right)) {
			if (playerInput.isKeyPressed(sp_right) && playerInput.isKeyDown(sp_right)) {
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
		if (playerInput.keyState(sp_down)) {
			if (playerInput.isKeyPressed(sp_down) && playerInput.isKeyDown(sp_down)) {
				if (!in_menu) {
					tmp_input = knight->DOWN;
				}
				menu_y += 1;
			}
			crouch();
		}
		
		// UP BUTTON
		if (playerInput.keyState(sp_up)) {
			if (playerInput.isKeyPressed(sp_up) && playerInput.isKeyDown(sp_up)) {
				if (!in_menu) {
					tmp_input = knight->UP;
				}
				
				menu_y -= 1;
			}
			up();
		}
	
		// JUMP BUTTON
		if (playerInput.keyState(sp_jump)) {
			if (playerInput.isKeyPressed(sp_jump) && playerInput.isKeyDown(sp_jump) && !in_menu) {
				tmp_input = knight->JUMP;
			}
			jump();
		}
		
		// ACTION BUTTON
		if (playerInput.keyState(sp_action)) {
			if (playerInput.isKeyPressed(sp_action) && playerInput.isKeyDown(sp_action) && !in_menu) {
				tmp_input = knight->ACTION;
			}
			basicAttack();
		}
		
		// MENU BUTTON
		if (playerInput.keyState(sp_menu)) {
		}

	} else { // MULTIPLAYER MAPPINGS
		// MOVE LEFT
		if (playerInput.keyState(mp_left)) {
			if (playerInput.isKeyPressed(mp_left) && playerInput.isKeyDown(mp_left)) {
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
		if (playerInput.keyState(mp_right)) {
			if (playerInput.isKeyPressed(mp_right) && playerInput.isKeyDown(mp_right)) {
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
		if (playerInput.keyState(mp_down)) {
			if (playerInput.isKeyPressed(mp_down) && playerInput.isKeyDown(mp_down)) {
				if (!in_menu) {
					tmp_input = knight->DOWN;
				}
				menu_y += 1;
			}
			crouch();
		}
		
		// UP BUTTON
		if (playerInput.keyState(mp_up)) {
			if (playerInput.isKeyPressed(mp_up) && playerInput.isKeyDown(mp_up)) {
				if (!in_menu) {
					tmp_input = knight->UP;
				}
				menu_y -= 1;
			}
			up();
		}
	
		// JUMP BUTTON
		if (playerInput.keyState(mp_jump)) {
			if (playerInput.isKeyPressed(mp_jump) && playerInput.isKeyDown(mp_jump) && !in_menu) {
				tmp_input = knight->JUMP;
			}
			jump();
		}
		
		// ACTION BUTTON
		if (playerInput.keyState(mp_action)) {
			if (playerInput.isKeyPressed(mp_action) && playerInput.isKeyDown(mp_action) && !in_menu) {
				tmp_input = knight->ACTION;
			}
			basicAttack();
		}
		
		// MENU BUTTON
		if (playerInput.keyState(mp_menu)) {
		}
	}
	
	if (!in_menu) {
		// NOTE(juha): Goes through all the special combos.
		for (int i = 0; i < moves_amount; ++i) {
			(*moves)[i].tmp_input = tmp_input;

			if ((*moves)[i].keys.size() > 0) {
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

				if ((*moves)[i].keys.size() == (*moves)[i].state) {
					(*moves)[i].executing = true;
					(*moves)[i].state = 0;
				}
			}
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
		}
	} else {

	}
}

void PlayerController::basicAttack()
{
	if (!in_menu) {
		attacking = true;
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
