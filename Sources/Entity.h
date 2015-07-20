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
	Entity() {};
	~Entity() {};
	virtual void defineAnimation(Animation *new_animation) = 0;

private:
};

#endif //__ENTITY_H_DEFINED__