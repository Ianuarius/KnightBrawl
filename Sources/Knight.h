/**
 * Knight.h
 *
 */

#ifndef __KNIGHT_H_DEFINED__
#define __KNIGHT_H_DEFINED__

#include "Animation.h"
#include "PugiXML.h"
#include "Window.h"

class Knight
{
public:
	Knight(Window *window, int knight_number);
	Animation *getAnimations(int animation);
	float getSpeed();
	float getJump();

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
	std::vector<Animation*> animations;
	std::string truename;
	std::string knightname;
	float speed;
	float jump;
	int hitpoints;

	pugi::xml_parse_result knight_result, roster_result;
	pugi::xml_document knight_document, roster_document;
};

#endif //__KNIGHT_H_DEFINED__