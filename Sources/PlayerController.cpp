/**
 * PlayerController.cpp
 *
 */

#include "PlayerController.h"

// http://wiki.gp2x.org/articles/s/d/l/SDL_Joystick_mapping.html
  
PlayerController::PlayerController(SDL_Point start_position, bool multiplayer, int player):
	location(start_position),
	multiplayer(multiplayer)
{
	std::string filename = "Controls.xml";
	result = controlsDocument.load_file(filename.c_str());
	SinglePlayerMappings = controlsDocument.child("profile").find_child_by_attribute("name", "singleplayer");
	MultiPlayerMappings = controlsDocument.child("profile").find_child_by_attribute("controller", std::to_string(player).c_str());

	if (!result) {
		printf("Failed to load controls file: %s.\n", filename.c_str());
		return;
	}
}

void PlayerController::update()
{
	if (!multiplayer) { // SINGLE PLAYER MAPPINGS
		// MOVE LEFT
		if (Input::keyState(fieldTypeParser.ParseSomeEnum
			(SinglePlayerMappings.find_child_by_attribute("name", "move_left").attribute("keyboard").value()))) {
			location.x -= 10;
		}
		
		// MOVE RIGHT
		if (Input::keyState(fieldTypeParser.ParseSomeEnum
			(SinglePlayerMappings.find_child_by_attribute("name", "move_right").attribute("keyboard").value()))) {
			location.x += 10;
		}
		
		// CROUCH BUTTON
		if (Input::keyState(fieldTypeParser.ParseSomeEnum
			(SinglePlayerMappings.find_child_by_attribute("name", "crouch").attribute("keyboard").value()))) {
			location.y += 10;
		}
		
		// UP BUTTON
		if (Input::keyState(fieldTypeParser.ParseSomeEnum
			(SinglePlayerMappings.find_child_by_attribute("name", "up").attribute("keyboard").value()))) {
			location.y -= 10;
		}
	
		// JUMP BUTTON
		if (Input::keyState(fieldTypeParser.ParseSomeEnum
			(SinglePlayerMappings.find_child_by_attribute("name", "jump").attribute("keyboard").value()))) {
			printf("jump\n");
		}
		
		// ACTION BUTTON
		if (Input::keyState(fieldTypeParser.ParseSomeEnum
			(SinglePlayerMappings.find_child_by_attribute("name", "action").attribute("keyboard").value()))) {
			printf("action\n");
		}
		
		// MENU BUTTON
		if (Input::keyState(fieldTypeParser.ParseSomeEnum
			(SinglePlayerMappings.find_child_by_attribute("name", "menu").attribute("keyboard").value()))) {
			printf("menu\n");
		}
	} else { // MULTIPLAYER MAPPINGS
		// MOVE LEFT
		if (Input::keyState(fieldTypeParser.ParseSomeEnum
			(MultiPlayerMappings.find_child_by_attribute("name", "move_left").attribute("keyboard").value()))) {
			location.x -= 10;
		}
		
		// MOVE RIGHT
		if (Input::keyState(fieldTypeParser.ParseSomeEnum
			(MultiPlayerMappings.find_child_by_attribute("name", "move_right").attribute("keyboard").value()))) {
			location.x += 10;
		}
		
		// CROUCH BUTTON
		if (Input::keyState(fieldTypeParser.ParseSomeEnum
			(MultiPlayerMappings.find_child_by_attribute("name", "crouch").attribute("keyboard").value()))) {
			location.y += 10;
		}
		
		// UP BUTTON
		if (Input::keyState(fieldTypeParser.ParseSomeEnum
			(MultiPlayerMappings.find_child_by_attribute("name", "up").attribute("keyboard").value()))) {
			location.y -= 10;
		}
	
		// JUMP BUTTON
		if (Input::keyState(fieldTypeParser.ParseSomeEnum
			(MultiPlayerMappings.find_child_by_attribute("name", "jump").attribute("keyboard").value()))) {
			printf("jump\n");
		}
		
		// ACTION BUTTON
		if (Input::keyState(fieldTypeParser.ParseSomeEnum
			(MultiPlayerMappings.find_child_by_attribute("name", "action").attribute("keyboard").value()))) {
			printf("action\n");
		}
		
		// MENU BUTTON
		if (Input::keyState(fieldTypeParser.ParseSomeEnum
			(MultiPlayerMappings.find_child_by_attribute("name", "menu").attribute("keyboard").value()))) {
			printf("menu\n");
		}
	}
}

SDL_Point PlayerController::getLocation()
{
	return location;
}

int PlayerController::getDirection()
{
	return facing;
}