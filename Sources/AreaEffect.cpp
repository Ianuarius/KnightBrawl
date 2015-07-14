/**
 * AreaEffect.cpp
 *
 */

#include "AreaEffect.h"

AreaEffect::AreaEffect():
	Entity(),
	executing(false),
	state(0),
	tmp_input(9999),
	hitbox(0, 0, 0, 0)
{

}

void AreaEffect::defineAnimation(Animation *new_animation)
{
	animation = new_animation;
}