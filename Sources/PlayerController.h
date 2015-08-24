/**
 * PlayerController.h
 *
 */

#ifndef __PLAYERCONTROLLER_H_DEFINED__
#define __PLAYERCONTROLLER_H_DEFINED__

#include <string>
#include "Effect.h"
#include "Input.h"
#include "Keymap.h"
#include "Knight.h"
#include "PugiXML.h"
#include "Rectangle.h"
#include "SDL.h"

#define GRAVITY 13
#define SPECIAL_MOVES 4

#define RESPAWN_TIME 3000

#define FACING_RIGHT 1
#define FACING_LEFT 2

const int JOYSTICK_DEAD_ZONE = 8000;

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
	void setPlayers(int *active_players);
	void commitMovement();
	void updateInput();
	void move(int x, int y);
	Knight *getKnight();
	void setGamepad(SDL_GameController *NewControllerHandle);

	int menu_x, menu_y;
	bool executing_combo, in_game;
	std::vector<SpecialCombo> *moves;
	std::vector<Effect *> movements;
	
	bool in_air, jumping, crouching, attacking, gamepad_ready;
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

	bool controller_locked, controller_in_use;
	bool has_attacked, has_jumped;
	Knight *knight;
	SDL_GameController *ControllerHandle;
	std::vector<SDL_Point> *positions;
	EnumParser<SDL_Scancode> fieldTypeParser;
	pugi::xml_document controlsDocument;
	pugi::xml_parse_result result;
	pugi::xml_node SinglePlayerMappings;
	pugi::xml_node MultiPlayerMappings;

	std::vector<int> combo_state;
	// std::vector<std::vector<int> > *special_combos;
	Timer deathTimer;
	Input playerInput;
	int moves_amount;
	int *players;
	int key_action, key_down, key_jump, key_left, key_menu, key_right, key_up;
	bool AButton_up, XButton_up, Up_up, Down_up, Left_up, Right_up;
	bool AButton, XButton, Up, Down, Left, Right;
	bool basic_attack;
	int combo_one_state, player;
	bool multiplayer, in_menu;
	int facing_direction;
	float speed;
	float acceleration;
	float targetVx;
	float stoppedThreshold;
};

#endif //__PLAYERCONTROLLER_H_DEFINED__