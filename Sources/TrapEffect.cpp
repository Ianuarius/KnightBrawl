/**
 * TrapEffect.cpp
 *
 */

#include "TrapEffect.h"

TrapEffect::TrapEffect():
	Entity(),
	executing(false),
	state(0),
	hitbox(0, 0, 0, 0)
{

}

void TrapEffect::defineAnimation(Animation *new_animation)
{
	animation = new_animation;
}