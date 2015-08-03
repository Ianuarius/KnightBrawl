/**
 * ProjectileSpawner.h
 *
 */

#ifndef __PROJECTILESPAWNER_H_DEFINED__
#define __PROJECTILESPAWNER_H_DEFINED__

#include <vector>
#include "Entity.h"
#include "Rectangle.h"

class ProjectileSpawner : public Entity
{
public:
	ProjectileSpawner();
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

#endif //__PROJECTILESPAWNER_H_DEFINED__