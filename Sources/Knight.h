/**
 * Knight.h
 *
 */

#ifndef __KNIGHT_H_DEFINED__
#define __KNIGHT_H_DEFINED__

#include <vector>
#include "Animation.h"
#include "AreaEffect.h"
#include "BuffEffect.h"
#include "Effect.h"
#include "Entity.h"
#include "Key.h"
#include "Projectile.h"
#include "ProjectileSpawner.h"
#include "PugiXML.h"
#include "Rectangle.h"
#include "Sound.h"
#include "SpecialCombo.h"
#include "TrapEffect.h"
#include "TrapSpawner.h"
#include "Window.h"

class Knight
{
public:
	Knight(Window *window, int knight_number);
	Animation *getAnimations(int animation);
	float getSpeed();
	float getJump();
	Sprite *getDecal();
	std::string getTruename();
	int getHitpoints();
	void damage(int amount);
	void respawn();
	Rectangle hitbox;
	bool alive, hit, jumping, landing, falling;
	std::vector<SpecialCombo> *getMoves();
	std::vector<Sound> *getSounds();
	
	bool is_landed, is_jumping;
	
	enum moves {
		FORWARD,
		BACKWARD,
		UP,
		DOWN,
		JUMP_MOVE,
		ACTION,
		MENU
	};
	
	enum attack_hitboxes {
		BASIC_HITBOX,
		MIDAIR_HITBOX,
		DOWNTHRUST_HITBOX,
		UPPERCUT_HITBOX,
		SPECIAL_I_HITBOX,
		SPECIAL_II_HITBOX,
		SPECIAL_III_HITBOX,
		SPECIAL_IV_HITBOX
	};

	enum possible_animations {
		IDLE=0,
		RUN,
		JUMP,
		ATTACK,
		BLOCK,
		CROUCH,
		DEATH,
		DODGE,
		DOWN_THRUST,
		HANGING,
		MID_AIR_BASIC_ATTACK,
		PUSHBACK,
		SPECIAL_I,
		SPECIAL_II,
		SPECIAL_III,
		SPECIAL_IV,
		THROW,
		UPPERCUT,

		ANIMATION_MAX
	};

private:
	int parseKey(std::string key);
	void parseSounds(pugi::xml_node *tmp_node);
	void parseEffects(pugi::xml_node *tmp_node);
	Animation *parseAction(pugi::xml_node *tmp_node);
	void parseActions(pugi::xml_node *tmp_node);

	std::vector<Animation*> animations;
	std::string truename;
	std::string knightname;
	Window *window;
	Sprite *decal;
	float speed;
	float jump;
	int hitpoints;
	std::vector<std::vector<Rectangle> > attack_hitboxes;
	std::vector<Projectile> projectiles;
	std::vector<ProjectileSpawner> projectile_spawners;
	std::vector<Sound> sounds;
	std::vector<SpecialCombo> moves;
	std::vector<SpecialCombo> special_combos;
	std::vector<TrapEffect> trap_effects;
	std::vector<TrapSpawner> trap_spawners;
	// std::vector<std::vector<int> > special_combos;

	pugi::xml_parse_result knight_result, roster_result;
	pugi::xml_document knight_document, roster_document;
};

#endif //__KNIGHT_H_DEFINED__