/**
 * Effect.h
 *
 */

#ifndef __EFFECT_H_DEFINED__
#define __EFFECT_H_DEFINED__

#include <vector>
#include "Entity.h"
#include "Rectangle.h"

class Effect
{
public:
	enum effect_types {
		SLOW,
		STUN,
		BURN,
		PUSHBACK,
		BUFF,
		DAMAGE_RETURN,
		MOVEMENT
	};
	
	enum effect_stats {
		DEFENSE,
		ATTACK,
		SPEED
	};
	
	enum effect_format {
		DURATION,
		COUNT,
		PRESSED
	};
	Effect();
	void defineAnimation(Animation *new_animation);
	
	Rectangle hitbox;
	Animation *animation;

	int angle;
	float duration;
	int damage;
	int format;
	int power;
	int range;
	int speed;
	int state;
	int stat;
	int time;
	int type;
	bool executing;
	int distance_travelled;
	std::vector<int> keys;
	int tmp_input;

private:
};

#endif //__EFFECT_H_DEFINED__