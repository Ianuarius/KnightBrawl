/**
 * Projectile.h
 *
 */

#ifndef __PROJECTILE_H_DEFINED__
#define __PROJECTILE_H_DEFINED__

#include <vector>
#include "Entity.h"
#include "Rectangle.h"

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
	
	Rectangle hitbox;
	Animation *animation;

	std::string name;
	int type;
	int range;
	int power;
	int speed;
	int collision_type;
	bool collision_destruct;
	bool executing;
	int state;


private:
};

#endif //__PROJECTILE_H_DEFINED__