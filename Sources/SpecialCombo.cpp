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
	disabled(false),
	frames(1),
	in_ground(false)
{
}

void SpecialCombo::defineAnimation(Animation *new_animation)
{
	animation = new_animation;
}

void SpecialCombo::setHitboxes()
{
	hitboxes.resize(frames);
}

void SpecialCombo::execute()
{

}