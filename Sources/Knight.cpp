/**
 * Knight.cpp
 *
 */

#include "Knight.h"

//vähän runkoa. Täydennetään kun tiedetään, mitä täydentää

Knight::Knight(Window *window):
	window(window),
	alive(true),
	specialbar(0),
	health(100)
{
	int specialbar = 0;
	int health = 100;
	if (health > 100){
		health = 100;
	}

	while (alive){
	
	timer.start();
	if (timer.getTicks() == 5000){
	(specialbar + 5);
	timer.stop();
	}
	if (specialbar > 1000){
		specialbar = 1000;
	}
	}
	
}

void Knight::attack()
{
}

void Knight::block()
{
}

void Knight::isDead()
{
	alive = false;
}

void Knight::damage(int amount)
{
	if (alive = true){
	(health - amount);
	if (health <= 0){
		isDead();
	}
	}
}

void Knight::update()
{
}

void Knight::updateAnimation()
{
}