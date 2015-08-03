/**
 * BuffEffect.cpp
 *
 */

#include "BuffEffect.h"

BuffEffect::BuffEffect():
	Entity(),
	executing(false),
	state(0),
	hitbox(0, 0, 0, 0)
{

}

void BuffEffect::defineAnimation(Animation *new_animation)
{
	animation = new_animation;
}