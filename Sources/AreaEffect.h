/**
 * AreaEffect.h
 *
 */

#ifndef __AREAEFFECT_H_DEFINED__
#define __AREAEFFECT_H_DEFINED__

#include <vector>
#include "Entity.h"
#include "Rectangle.h"

class AreaEffect : public Entity
{
public:
	AreaEffect();
	void defineAnimation(Animation *new_animation);
	
	Rectangle hitbox;
	Animation *animation;

	std::string name;
	int damage;
	bool executing;
	int state;

private:
};

#endif //__AREAEFFECT_H_DEFINED__