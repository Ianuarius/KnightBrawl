/**
 * SpecialCombo.cpp
 *
 */

#include "SpecialCombo.h"

SpecialCombo::SpecialCombo():
	Entity(),
	executing(false), start_execution(false),
	state(0),
	tmp_input(9999),
	hitbox(0, 0, 0, 0),
	disabled(false)
{

}

void SpecialCombo::defineAnimation(Animation *new_animation)
{
	animation = new_animation;
}

void SpecialCombo::execute()
{

}