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
	// NOTE(juha): Constructor for in-menu controls.
	PlayerController(bool multiplayer, int player, std::vector<SDL_Point> *positions);
	
	// NOTE(juha): Constructor for in-game controls.
	PlayerController(SDL_Point start_position, 
		bool multiplayer, int player, Knight *knight);

	int getDirection();
	void update();
	void commitMovement();
	void updateInput();
	int menu_x, menu_y;
	std::vector<SpecialCombo> *moves;
	
	bool in_air, jumping, crouching, attacking;
	float velocity_x, velocity_y;
	SDL_Point location;
	Rectangle boundbox, hitbox, desired, attack_hb;

private:
	void jump();
	void left();
	void right();
	void crouch();
	void up();
	void basicAttack();
	void parseMappedValues();

	Knight *knight;
	std::vector<SDL_Point> *positions;
	EnumParser<SDL_Scancode> fieldTypeParser;
	pugi::xml_document controlsDocument;
	pugi::xml_parse_result result;
	pugi::xml_node SinglePlayerMappings;
	pugi::xml_node MultiPlayerMappings;

	std::vector<int> combo_state;
	// std::vector<std::vector<int> > *special_combos;
	Timer specialOneTimer;
	Input playerInput;

	int moves_amount;
	int mp_action, mp_down, mp_jump, mp_left, mp_menu, mp_right, mp_up;
	int sp_action, sp_down, sp_jump, sp_left, sp_menu, sp_right, sp_up;
	int combo_one_state, player;
	bool multiplayer, in_menu;
	int facing_direction;
	float speed;
	float acceleration;
	float targetVx;
	float stoppedThreshold;
};

#endif //__PLAYERCONTROLLER_H_DEFINED__