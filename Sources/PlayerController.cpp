/**
 * PlayerController.cpp
 *
 */

#include "PlayerController.h"

// XBOX 360 Mapping:
// http://wiki.gp2x.org/articles/s/d/l/SDL_Joystick_mapping.html
  
PlayerController::PlayerController(SDL_Point start_position, bool multiplayer, int player, Knight *knight):
	multiplayer(multiplayer),
	location(start_position),
	boundbox(location.x - 25, location.y - 50, 50, 50),
	hitbox(boundbox.x + ((50 / 2) - (26 / 2)), boundbox.y + (50 - 26), 26, 26),
	desired(hitbox),
	acceleration(0.7f), stoppedThreshold(acceleration/7),
	velocity_x(0), velocity_y(0),
	knight(knight),
	targetVx(0),
	facing_direction(FACING_RIGHT),
	speed(knight->getSpeed()),
	special_combos(knight->getSpecialCombos()),
	combo_one_state(0)
{
	std::string filename = "Controls.xml";
	result = controlsDocument.load_file(filename.c_str());
	SinglePlayerMappings = 
		controlsDocument.child("profile").
		find_child_by_attribute("name", "singleplayer");
	MultiPlayerMappings = 
		controlsDocument.child("profile").
		find_child_by_attribute("controller", std::to_string(player).c_str());
	
	// NOTE(juha): Initialization
	in_air =			true;
	jumping =			false;
	crouching =			false;
	in_special_one =	false;
	in_special_two =	false;
	in_special_three =	false;
	in_special_four =	false;
	
	
	for (int i = 0; i < SPECIAL_MOVES; ++i) {
		combo_state.push_back(0);
	}

	
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

	if (!result) {
		printf("Failed to load controls file: %s.\n", filename.c_str());
		return;
	}
}

void PlayerController::update()
{
	// Gravitational stuff
	
	velocity_y += GRAVITY * (16.f / 1000);
	
	desired.y += velocity_y;
	crouching = false;

	if (velocity_y >= 7) {
		velocity_y = 7;
	}
	
	if (!in_air) {
		jumping = false;
	}

	int tmp_input = 9999;

	if (!multiplayer) { // SINGLE PLAYER MAPPINGS
		// MOVE LEFT
		if (playerInput.keyState(sp_left)) {

			if (playerInput.isKeyPressed(sp_left) && playerInput.isKeyDown(sp_left)) {
				if (FACING_LEFT) {
					tmp_input = knight->FORWARD;
				} else {
					tmp_input = knight->BACKWARD;
				}
			}

			left();
		}
		
		// MOVE RIGHT
		if (playerInput.isKeyPressed(sp_right)) {

			if (playerInput.keyPressed(sp_right)) {
				if (FACING_RIGHT) {
					tmp_input = knight->FORWARD;
				} else {
					tmp_input = knight->BACKWARD;
				}
			}

			right();
		}
		
		// CROUCH BUTTON
		if (playerInput.keyState(sp_down)) {
			crouch();
		}
		
		// UP BUTTON
		if (playerInput.keyState(sp_up)) {
			up();
		}
	
		// JUMP BUTTON
		if (playerInput.keyState(sp_jump)) {
			jump();
		}
		
		// ACTION BUTTON
		if (playerInput.keyState(sp_action)) {
			action();
		}
		
		// MENU BUTTON
		if (playerInput.keyState(sp_menu)) {
			printf("menu\n");
		}

	} else { // MULTIPLAYER MAPPINGS
		// MOVE LEFT
		if (playerInput.keyState(mp_left)) {

			if (playerInput.isKeyPressed(mp_left) && playerInput.isKeyDown(mp_left)) {
				if (facing_direction == FACING_LEFT) {
					tmp_input = knight->FORWARD;
				} else {
					tmp_input = knight->BACKWARD;
				}
			}

			left();
		}
		
		// MOVE RIGHT
		if (playerInput.keyState(mp_right)) {
			
			if (playerInput.isKeyPressed(mp_right) && playerInput.isKeyDown(mp_right)) {
				if (facing_direction == FACING_RIGHT) {
					tmp_input = knight->FORWARD;
				} else {
					tmp_input = knight->BACKWARD;
				}
			}

			right();
		}
		
		// CROUCH BUTTON
		if (playerInput.keyState(mp_down)) {
			
			if (playerInput.isKeyPressed(mp_down) && playerInput.isKeyDown(mp_down)) {
				tmp_input = knight->DOWN;
			}

			crouch();
		}
		
		// UP BUTTON
		if (playerInput.keyState(mp_up)) {
			
			if (playerInput.isKeyPressed(mp_up) && playerInput.isKeyDown(mp_up)) {
				tmp_input = knight->UP;
			}

			up();
		}
	
		// JUMP BUTTON
		if (playerInput.keyState(mp_jump)) {
			
			if (playerInput.isKeyPressed(mp_jump) && playerInput.isKeyDown(mp_jump)) {
				tmp_input = knight->JUMP;
			}

			jump();
		}
		
		// ACTION BUTTON
		if (playerInput.keyState(mp_action)) {
			
			if (playerInput.isKeyPressed(mp_action) && playerInput.isKeyDown(mp_action)) {
				tmp_input = knight->ACTION;
			}

			action();
		}
		
		// MENU BUTTON
		if (playerInput.keyState(mp_menu)) {
		}
	}

	if (jumping) {
		jump();
	}
	
	// TODO(juha): Käy kaikki combot läpi

	for (int i = 0; i < SPECIAL_MOVES; ++i) {
		(*special_combos)[i].tmp_input = tmp_input;

		if ((*special_combos)[i].keys[0] == knight->FORWARD &&
			(*special_combos)[i].tmp_input == knight->BACKWARD &&
			(*special_combos)[i].state == 0) {
			(*special_combos)[i].tmp_input = knight->FORWARD;
		}

		if ((*special_combos)[i].keys[(*special_combos)[i].state] == (*special_combos)[i].tmp_input &&
			(*special_combos)[i].executing == false) {
			(*special_combos)[i].state++;
		} else if ((*special_combos)[i].tmp_input == 9999) {

		} else {
			(*special_combos)[i].state = 0;
		}

		if ((*special_combos)[i].keys.size() == (*special_combos)[i].state) {
			(*special_combos)[i].executing = true;
			(*special_combos)[i].state = 0;
		}
	}
	
	velocity_x += ((targetVx - velocity_x) * acceleration) * 0.16667f;
	
	if (fabs(velocity_x) > speed) {
		velocity_x = (velocity_x > 0) ? (speed+1) : -speed;
	}

	if (fabs(velocity_x) < stoppedThreshold) {
		velocity_x = 0;
	}
	
	// printf("%f\n", velocity_x);

	if (facing_direction == FACING_RIGHT) {
		desired.x += ceilf(fabs(velocity_x));
	} else {
		desired.x -= ceilf(fabs(velocity_x));
	}

	// desired.x += floorf(velocity_x);
	targetVx = 0;

	/*
	velocity_x += ((targetVx - velocity_x) * acceleration) * 0.16667f;
 
	if (fabs(velocity_x) > speed) {
		velocity_x = (velocity_x > 0) ? speed : -speed;
	}
 
	if (fabs(velocity_x) < stoppedThreshold) {
		velocity_x = 0;
	}
 
	velocity_x = ceilf(fabs(velocity_x));
 
	if (facing_direction == FACING_RIGHT) {
		desired.x += velocity_x;
	} else {
		desired.x -= velocity_x;
	}
 
	targetVx = 0;
	*/
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
	hitbox.x = boundbox.x + ((50 / 2) - (26 / 2));
	hitbox.y = boundbox.y + (50 - 26);
}

void PlayerController::up()
{
	// velocity_y -= speed * 0.1f;
}

void PlayerController::action()
{
}

// IDLE

// RUN
void PlayerController::left()
{
	targetVx = -(speed * acceleration);
	facing_direction = FACING_LEFT;
}

void PlayerController::right()
{
	targetVx = (speed * acceleration);
	facing_direction = FACING_RIGHT;
}

// JUMP
void PlayerController::jump()
{
	if (in_air == false) {
		velocity_y -= knight->getJump();
		in_air = true;
		jumping = true;
	}
}

// ATTACK
// BLOCK

// CROUCH
void PlayerController::crouch()
{
	if (!jumping && !in_air) {
		crouching = true;
		velocity_x = 0;
		targetVx = 0;
	}
	// desired.y += speed;
}

// DEATH
// DODGE
// DOWN_THRUST
// HANGING
// MID_AIR_BASIC_ATTACK
// PUSHBACK

// THROW
// UPPERCUT
