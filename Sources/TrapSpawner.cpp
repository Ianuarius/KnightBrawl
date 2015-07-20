/**
 * TrapSpawner.cpp
 *
 */

#include "TrapSpawner.h"

TrapSpawner::TrapSpawner():
	Entity()
{

}

void TrapSpawner::defineAnimation(Animation *new_animation)
{
	animation = new_animation;
}