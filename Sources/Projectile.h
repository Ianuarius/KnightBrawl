/**
 * Projectile.h
 *
 */

#ifndef __PROJECTILE_H_DEFINED__
#define __PROJECTILE_H_DEFINED__

#include <vector>
#include <math.h>
#include "Entity.h"
#include "Rectangle.h"

#define PI		3.14159265358979323846

class Projectile : public Entity
{
public:
	enum projetcile_types {
		TYPE_LINEAR,
		TYPE_ARC,
		TYPE_MISC
	};
	
	enum collision_types {
		COLLISION_ALL,
		COLLISION_PLAYER,
		COLLISION_WALL
	};

	Projectile();
	void defineAnimation(Animation *new_animation);
	void update();
	
	Rectangle hitbox;
	SDL_Point location;
	Animation *animation;

	std::string name;
	int type;
	int direction;
	int angle;
	int range;
	int power;
	int speed;
	int y_offset, x_offset;
	double x_coordinate, y_coordinate;
	int loops;
	int drop_speed;
	bool collision;
	int collision_type;
	bool collision_destruct;
	bool executing, hit;
	int player;
	int state;


private:
};

#endif //__PROJECTILE_H_DEFINED__