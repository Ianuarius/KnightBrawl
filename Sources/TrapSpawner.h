/**
 * TrapSpawner.h
 *
 */

#ifndef __TRAPSPAWNER_H_DEFINED__
#define __TRAPSPAWNER_H_DEFINED__

#include <vector>
#include "Entity.h"
#include "Rectangle.h"

class TrapSpawner : public Entity
{
public:
	TrapSpawner();
	void defineAnimation(Animation *new_animation);
	
	Animation *animation;

	std::string type;
	int amount;
	int angle;
	int angle_interval;
	int delay;
	int delay_interval;

private:
};

#endif //__TRAPSPAWNER_H_DEFINED__