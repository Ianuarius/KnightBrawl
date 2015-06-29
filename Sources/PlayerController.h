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
#define SPECIAL_MOVES 4

#define FACING_RIGHT 1
#define FACING_LEFT 2

class PlayerController
{
public:
	PlayerController(SDL_Point start_position, 
		bool multiplayer, int player, Knight *knight);
	int getDirection();
	void update();
	void commitMovement();
	void updateInput();
	
	bool in_air, jumping, crouching, in_special_one,
		 in_special_two, in_special_three, in_special_four;
	float velocity_x, velocity_y;
	SDL_Point location;
	Rectangle boundbox, hitbox, desired;

private:
	void specialOne();
	void specialTwo();
	void specialThree();
	void specialFour();
	void jump();
	void left();
	void right();
	void crouch();
	void up();
	void action();

	Knight *knight;
	EnumParser<SDL_Scancode> fieldTypeParser;
	pugi::xml_document controlsDocument;
	pugi::xml_parse_result result;
	pugi::xml_node SinglePlayerMappings;
	pugi::xml_node MultiPlayerMappings;

	std::vector<int> *special_one_combo;
	std::vector<int> *special_two_combo;
	std::vector<int> *special_three_combo;
	std::vector<int> *special_four_combo;
	Timer specialOneTimer;
	Input playerInput;

	int mp_action, mp_down, mp_jump, mp_left, mp_menu, mp_right, mp_up;
	int sp_action, sp_down, sp_jump, sp_left, sp_menu, sp_right, sp_up;
	int combo_one_state;
	bool multiplayer;
	int facing_direction;
	float speed;
	float acceleration;
	float targetVx;
	float stoppedThreshold;
};

#endif //__PLAYERCONTROLLER_H_DEFINED__