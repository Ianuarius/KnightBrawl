/**
 * PlayerController.h
 *
 */

#ifndef __PLAYERCONTROLLER_H_DEFINED__
#define __PLAYERCONTROLLER_H_DEFINED__

#include "Input.h"
#include "SDL.h"
#include "PugiXML.h"
#include "Keymap.h"
#include "Rectangle.h"
#include <string>

#define GRAVITY 13

enum Direction {RIGHT, LEFT};

class PlayerController
{
public:
	PlayerController(SDL_Point start_position, 
		bool multiplayer, int player);
	int getDirection();
	void update();
	void jump();
	void left();
	void right();
	void crouch();
	void up();
	void action();
	
	bool in_air;
	float velocity_x, velocity_y;
	SDL_Point location;
	Rectangle boundbox, hitbox, old_bound;

private:
	EnumParser<SDL_Scancode> fieldTypeParser;
	pugi::xml_document controlsDocument;
	pugi::xml_parse_result result;
	pugi::xml_node SinglePlayerMappings;
	pugi::xml_node MultiPlayerMappings;
	bool multiplayer;
	int facing;
	float speed;
	float acceleration;
};

#endif //__PLAYERCONTROLLER_H_DEFINED__