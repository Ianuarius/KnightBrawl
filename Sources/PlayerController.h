/**
 * PlayerController.h
 *
 * Controls the player character movements and actions.
 *
 */

#ifndef PLAYERCONTROLLER_H_DEFINED
#define PLAYERCONTROLLER_H_DEFINED

#include <string>
#include "Effect.h"
#include "Input.h"
#include "Keymap.h"
#include "Knight.h"
#include "PugiXML.h"
#include "Rectangle.h"
#include "SDL.h"
#include "SFX.h"

#define GRAVITY 13

// In milliseconds how long it takes for a player to respawn.
#define RESPAWN_TIME 3000

#define FACING_RIGHT 1
#define FACING_LEFT 2

class PlayerController
{
public:
	// NOTE(juha): Constructor for in-menu controls.
	// Parameters:
		// multiplayer	Tells if there are multiple players on the same PC.
		// player		Player index.
		// positions	(DEPRECATED) Player positions in the menu grid.
	PlayerController(bool multiplayer,
					 int player,
					 std::vector<SDL_Point> *positions);
	
	// NOTE(juha): Constructor for in-game controls.
	// Parameters:
		// start_position	Location where the knight will spawn.
		// multiplayer		Tells if there are multiple players on the same PC.
		// player			Player index.
		// knight			Which knight the player is playing.
	PlayerController(SDL_Point start_position, 
					 bool multiplayer,
					 int player,
					 Knight *knight);

	void update();

	// Sets the player location to be the desired location.
	void commitMovement();
	void updateInput();

	// Knocks the knight back to the given angle.
	void knockBack(int attack_direction, int attack_angle, int attack_power);

	// Sets character's values back to idle when a move ends.
	void stopAttack();

	void setPlayers(int *active_players);
	void setGamepad(SDL_GameController *NewControllerHandle);
	Knight *getKnight();
	int getDirection();
	
	SDL_Point location;		// Bottom center of the knight sprite.
	Rectangle boundbox,		// Size and location of the knight sprite.
			  hitbox,		// Size and loc of the hitbox of the knight.
			  desired,		// Uncommitted hitbox size and loc of the knight.  
			  attack_hb;	// Size and loc of the hitbox of the attack.

	std::vector<SpecialCombo> *moves;
	std::vector<Effect> movements;
	
	int menu_x, menu_y;
	int attack_w, attack_h;
	int pushback_power, pushback_angle;
	bool executing_combo, in_game;
	bool in_air, jumping, crouching, attacking, gamepad_ready;
	float velocity_x, velocity_y;

private:
	void jump();
	void left();
	void right();
	void crouch();
	void up();
	void basicAttack();

	// Goes throught Controls.xml and parses controls from there to the game.
	void parseMappedValues();


	void doAction(int action);

	Knight						*knight;
	SDL_GameController			*ControllerHandle;
	EnumParser<SDL_Scancode>	fieldTypeParser;	// For parsing inputs.
	Timer						deathTimer;
	Input						playerInput;
	SFX							sfx_select2;
	
	std::vector<SDL_Point> *positions;
	std::vector<int> combo_state;
	// std::vector<std::vector<int> > *special_combos;

	pugi::xml_document controlsDocument;
	pugi::xml_parse_result result;
	pugi::xml_node SinglePlayerMappings;
	pugi::xml_node MultiPlayerMappings;

	int *players;

	// Parsed values for inputs.
	int key_action, key_down, key_jump, key_left, key_menu, key_right, key_up;

	int		player;
	int		facing_direction;
	float	speed;
	float	acceleration;
	float	targetVx;
	float	stoppedThreshold;
	bool	multiplayer, in_menu;
	bool	AButton_up, XButton_up, Up_up, Down_up, Left_up, Right_up;
	bool	AButton,    XButton,    Up,    Down,    Left,    Right;
	bool	basic_attack;
	bool	controller_locked, controller_in_use;
	bool	has_attacked, has_jumped;
};

#endif //PLAYERCONTROLLER_H_DEFINED