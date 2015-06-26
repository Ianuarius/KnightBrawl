/**
 * PlayerController.h
 *
 */

#ifndef __PLAYERCONTROLLER_H_DEFINED__
#define __PLAYERCONTROLLER_H_DEFINED__

#include <string>
#include "Input.h"
#include "Keymap.h"
#include "Knight.h"
#include "PugiXML.h"
#include "Rectangle.h"
#include "SDL.h"

#define GRAVITY 13

#define FACING_RIGHT 1
#define FACING_LEFT 2

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
	void commitMovement();
	
	bool in_air, jumping;
	float velocity_x, velocity_y;
	SDL_Point location;
	Rectangle boundbox, hitbox, desired;

private:
	EnumParser<SDL_Scancode> fieldTypeParser;
	pugi::xml_document controlsDocument;
	pugi::xml_parse_result result;
	pugi::xml_node SinglePlayerMappings;
	pugi::xml_node MultiPlayerMappings;
	bool multiplayer;
	int facing_direction;
	float speed;
	float acceleration;
	float targetVx;
	float stoppedThreshold;
};

#endif //__PLAYERCONTROLLER_H_DEFINED__