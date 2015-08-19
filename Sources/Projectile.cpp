/**
 * Projectile.cpp
 *
 */

#include "Projectile.h"

Projectile::Projectile():
	Entity(),
	executing(false), hit(false), collision(false),
	state(0),
	direction(1),
	drop_speed(0),
	hitbox(0, 0, 0, 0),
	x_offset(0), y_offset(0), loops(0)
{

}

void Projectile::defineAnimation(Animation *new_animation)
{
	animation = new_animation;
}

void Projectile::update()
{
	if (angle > -90) {
		angle -= drop_speed;
	}
	double new_x = speed * cos(angle * PI / 180);
	double new_y = speed * sin(angle * PI / 180);
	
	if (direction == 1) {
		x_coordinate += new_x;
	} else {
		x_coordinate -= new_x;
	}

	y_coordinate -= new_y;

	location.x = x_coordinate;
	location.y = y_coordinate;

	hitbox.x = location.x;
	hitbox.y = location.y;
}