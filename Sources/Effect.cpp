/**
 * Effect.cpp
 *
 */

#include "Effect.h"

Effect::Effect():
	executing(false),
	state(0),
	tmp_input(9999),
	hitbox(0, 0, 0, 0)
{

}

void Effect::defineAnimation(Animation *new_animation)
{
	animation = new_animation;
}