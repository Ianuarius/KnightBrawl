/**
 * Projectile.cpp
 *
 */

#include "Projectile.h"

Projectile::Projectile():
	Entity(),
	executing(false), hit(false),
	state(0),
	hitbox(0, 0, 0, 0)
{

}

void Projectile::defineAnimation(Animation *new_animation)
{
	animation = new_animation;
}

void Projectile::update(int direction)
{
	double new_x = speed * cos(angle);
	double new_y = speed * sin(angle);
	
	if (direction == 1) {
		location.x += new_x;
	} else {
		location.x -= new_x;
	}

	location.y += new_y;

	hitbox.x = location.x;
	hitbox.y = location.y;
}