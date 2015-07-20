/**
 * ProjectileSpawner.cpp
 *
 */

#include "ProjectileSpawner.h"

ProjectileSpawner::ProjectileSpawner():
	Entity()
{

}

void ProjectileSpawner::defineAnimation(Animation *new_animation)
{
	animation = new_animation;
}