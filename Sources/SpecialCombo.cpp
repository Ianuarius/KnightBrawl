/**
 * SpecialCombo.cpp
 *
 */

#include "SpecialCombo.h"

SpecialCombo::SpecialCombo():
	Entity(),
	executing(false),
	state(0),
	tmp_input(9999),
	hitbox(0, 0, 0, 0)
{

}

void SpecialCombo::defineAnimation(Animation *new_animation)
{
	animation = new_animation;
}