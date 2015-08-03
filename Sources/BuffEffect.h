/**
 * BuffEffect.h
 *
 */

#ifndef __BUFFEFFECT_H_DEFINED__
#define __BUFFEFFECT_H_DEFINED__

#include <vector>
#include "Entity.h"
#include "Rectangle.h"

class BuffEffect : public Entity
{
public:
	enum collision_types {
		COLLISION_ALL,
		COLLISION_PLAYER,
		COLLISION_WALL
	};

	BuffEffect();
	void defineAnimation(Animation *new_animation);
	
	Rectangle hitbox;
	Animation *animation;

	std::string name;
	int damage;
	int collision_type;
	bool collision_destruct;
	bool executing;
	int state;

private:
};

#endif //__BUFFEFFECT_H_DEFINED__