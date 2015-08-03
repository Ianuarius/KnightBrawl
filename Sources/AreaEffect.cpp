/**
 * AreaEffect.cpp
 *
 */

#include "AreaEffect.h"

AreaEffect::AreaEffect():
	Entity(),
	executing(false),
	state(0),
	hitbox(0, 0, 0, 0)
{

}

void AreaEffect::defineAnimation(Animation *new_animation)
{
	animation = new_animation;
}