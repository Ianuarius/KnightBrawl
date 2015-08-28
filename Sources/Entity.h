/**
 * Entity.h
 *
 */

#ifndef __ENTITY_H_DEFINED__
#define __ENTITY_H_DEFINED__

#include <vector>
#include "Animation.h"

class Entity
{
public:
	Entity() {hit = false;};
	~Entity() {};
	virtual void defineAnimation(Animation *new_animation) = 0;
	bool hit;
	virtual void setHit(bool new_hit) {hit = new_hit;}
	virtual bool getHit() {return hit;}

private:
};

#endif //__ENTITY_H_DEFINED__