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

	inline int getDirection();
	void update();
	inline void commitMovement();
	inline void updateInput();
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
	void doAction(int action);

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
	int key_action, key_down, key_jump, key_left, key_menu, key_right, key_up;
	int combo_one_state, player;
	bool multiplayer, in_menu;
	int facing_direction;
	float speed;
	float acceleration;
	float targetVx;
	float stoppedThreshold;
};

#endif //__PLAYERCONTROLLER_H_DEFINED__