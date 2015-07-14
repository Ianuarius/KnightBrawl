/**
 * Projectile.cpp
 *
 */

#include "Projectile.h"

Projectile::Projectile():
	Entity(),
	executing(false),
	state(0),
	hitbox(0, 0, 0, 0)
{

}

void Projectile::defineAnimation(Animation *new_animation)
{
	animation = new_animation;
}