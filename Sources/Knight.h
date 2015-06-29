/**
 * Knight.h
 *
 */

#ifndef __KNIGHT_H_DEFINED__
#define __KNIGHT_H_DEFINED__

#include <vector>
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
	std::vector<int> *getSpecialOneCombo();
	std::vector<int> *getSpecialTwoCombo();
	std::vector<int> *getSpecialThreeCombo();
	std::vector<int> *getSpecialFourCombo();
	
	enum moves {
		FORWARD,
		BACKWARD,
		UP,
		DOWN,
		JUMP_MOVE,
		ACTION,
		MENU
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
	std::vector<Animation*> animations;
	std::string truename;
	std::string knightname;
	Window *window;
	float speed;
	float jump;
	int hitpoints;
	
	// Each element of the vector has one button of the
	// combo saved in order.
	std::vector<int> special_one_combo,
					 special_two_combo,
					 special_three_combo,
					 special_four_combo;

	pugi::xml_parse_result knight_result, roster_result;
	pugi::xml_document knight_document, roster_document;
};

#endif //__KNIGHT_H_DEFINED__