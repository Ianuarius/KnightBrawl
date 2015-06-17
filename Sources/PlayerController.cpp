/**
 * PlayerController.cpp
 *
 */

#include "PlayerController.h"

// XBOX 360 Mapping:
// http://wiki.gp2x.org/articles/s/d/l/SDL_Joystick_mapping.html
  
PlayerController::PlayerController(SDL_Point start_position, bool multiplayer, int player):
	multiplayer(multiplayer),
	location(start_position),
	boundbox(location.x - 25, location.y - 50, 50, 50),
	hitbox(boundbox.x + ((50 / 2) - (26 / 2)), boundbox.y + (50 - 26), 26, 26),
	old_bound(boundbox),
	in_air(false)
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
}

void PlayerController::update()
{
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

	boundbox.x = location.x - 25;
	boundbox.y = location.y - 50;
	hitbox.x = boundbox.x + ((50 / 2) - (26 / 2));
	hitbox.y = boundbox.y + (50 - 26);

}

int PlayerController::getDirection()
{
	return facing;
}

void PlayerController::jump()
{
	printf("jump\n");
}

void PlayerController::crouch()
{
	location.y += 10;
}

void PlayerController::up()
{
	location.y -= 10;
}

void PlayerController::action()
{
	printf("action\n");
}

void PlayerController::left()
{
	location.x -= 10;
}

void PlayerController::right()
{
	location.x += 10;
}