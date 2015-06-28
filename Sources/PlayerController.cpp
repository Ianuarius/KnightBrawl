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
	acceleration(0.8), stoppedThreshold(acceleration/3),
	velocity_x(0), velocity_y(0),
	knight(knight),
	targetVx(0),
	facing_direction(FACING_RIGHT),
	speed(knight->getSpeed())
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
	in_air =	true;
	jumping =	false;
	crouching = false;

	moveTimer.start();

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

	velocity_x += ((targetVx - velocity_x) * acceleration) * 0.16667f;

	if (fabs(velocity_x) > speed) {
		velocity_x = (velocity_x > 0) ? speed : -speed;
	}

	if (fabs(velocity_x) < stoppedThreshold) {
		velocity_x = 0;
	}

	desired.x += floorf(velocity_x);
	targetVx = 0;

	if (!multiplayer) { // SINGLE PLAYER MAPPINGS
		// MOVE LEFT
		if (Input::keyState(fieldTypeParser.ParseSomeEnum(SinglePlayerMappings.
			find_child_by_attribute("name", "move_left").attribute("keyboard").value()))) {
			left();
		}
		
		// MOVE RIGHT
		if (Input::keyState(fieldTypeParser.ParseSomeEnum(SinglePlayerMappings.
			find_child_by_attribute("name", "move_right").attribute("keyboard").value()))) {
			right();
		}
		
		// CROUCH BUTTON
		if (Input::keyState(fieldTypeParser.ParseSomeEnum(SinglePlayerMappings.
			find_child_by_attribute("name", "crouch").attribute("keyboard").value()))) {
			crouch();
		}
		
		// UP BUTTON
		if (Input::keyState(fieldTypeParser.ParseSomeEnum(SinglePlayerMappings.
			find_child_by_attribute("name", "up").attribute("keyboard").value()))) {
			up();
		}
	
		// JUMP BUTTON
		if (Input::keyState(fieldTypeParser.ParseSomeEnum(SinglePlayerMappings.
			find_child_by_attribute("name", "jump").attribute("keyboard").value()))) {
			jump();
		}
		
		// ACTION BUTTON
		if (Input::keyState(fieldTypeParser.ParseSomeEnum(SinglePlayerMappings.
			find_child_by_attribute("name", "action").attribute("keyboard").value()))) {
			action();
		}
		
		// MENU BUTTON
		if (Input::keyState(fieldTypeParser.ParseSomeEnum(SinglePlayerMappings.
			find_child_by_attribute("name", "menu").attribute("keyboard").value()))) {
			printf("menu\n");
		}

	} else { // MULTIPLAYER MAPPINGS
		// MOVE LEFT
		if (Input::keyState(fieldTypeParser.ParseSomeEnum(MultiPlayerMappings.
			find_child_by_attribute("name", "move_left").attribute("keyboard").value()))) {
			left();
		}
		
		// MOVE RIGHT
		if (Input::keyState(fieldTypeParser.ParseSomeEnum(MultiPlayerMappings.
			find_child_by_attribute("name", "move_right").attribute("keyboard").value()))) {
			right();
		}
		
		// CROUCH BUTTON
		if (Input::keyState(fieldTypeParser.ParseSomeEnum(MultiPlayerMappings.
			find_child_by_attribute("name", "crouch").attribute("keyboard").value()))) {
			crouch();
		}
		
		// UP BUTTON
		if (Input::keyState(fieldTypeParser.ParseSomeEnum(MultiPlayerMappings.
			find_child_by_attribute("name", "up").attribute("keyboard").value()))) {
			up();
		}
	
		// JUMP BUTTON
		if (Input::keyState(fieldTypeParser.ParseSomeEnum(MultiPlayerMappings.
			find_child_by_attribute("name", "jump").attribute("keyboard").value()))) {
			jump();
		}
		
		// ACTION BUTTON
		if (Input::keyState(fieldTypeParser.ParseSomeEnum(MultiPlayerMappings.
			find_child_by_attribute("name", "action").attribute("keyboard").value()))) {
			action();
		}
		
		// MENU BUTTON
		if (Input::keyState(fieldTypeParser.ParseSomeEnum(MultiPlayerMappings.
			find_child_by_attribute("name", "menu").attribute("keyboard").value()))) {
			printf("menu\n");
		}
	}

	if (jumping) {
		jump();
	}

	// printf("%f\n", velocity_y);

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
	printf("%d\n", std::to_string(moveTimer.getTicks()).c_str());
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

// SPECIAL_I
void PlayerController::specialOne()
{
	// tietojen lukeminen xml:st‰

	// otetaan ajastimella aika 
	
	// animaation vaihto / bool
	// n‰ytet‰‰n vain kerran
}

// SPECIAL_II
// SPECIAL_III
// SPECIAL_IV
// THROW
// UPPERCUT
