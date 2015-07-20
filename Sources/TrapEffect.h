/**
 * TrapEffect.h
 *
 */

#ifndef __TRAPEFFECT_H_DEFINED__
#define __TRAPEFFECT_H_DEFINED__

#include <vector>
#include "Entity.h"
#include "Rectangle.h"

class TrapEffect : public Entity
{
public:
	enum effect_types {
		TYPE_PUSHBACK,
		TYPE_MISC
	};
	
	enum collision_types {
		COLLISION_ALL,
		COLLISION_PLAYER,
		COLLISION_WALL
	};

	TrapEffect();
	void defineAnimation(Animation *new_animation);
	
	Rectangle hitbox;
	Animation *animation;

	std::string name;
	int damage;
	int effect_type;
	int effect_power;
	int collision_type;
	bool collision_destruct;
	bool executing;
	int state;

private:
};

#endif //__TRAPEFFECT_H_DEFINED__