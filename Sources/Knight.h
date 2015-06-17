/**
 * Knight.h
 *
 */

#ifndef __KNIGHT_H_DEFINED__
#define __KNIGHT_H_DEFINED__

#include "Timer.h"
#include "Animation.h"
#include "Texture.h"
#include "Window.h"


class Knight
{
public:
	Knight(Window *window);

	void update();
	void updateAnimation();
	enum possible_animations {
		IDLE = 0,
		ATTACK,
		BLOCK,
		CROUCH,
		DEATH,
		DODGE,
		DOWNTHRUST,
		HANGING,
		JUMP,
		AIRATTACK,
		PUSHBACKTHROWN,
		RUN,
		SPECIAL1,
		SPECIAL2,
		SPECIAL3,
		SPECIAL4,
		THROW,
		UPPERCUT
	};


	void attack();
	void block();
	void damage(int amount);
	void isDead();

private:
	Window *window;

	Animation *currentAnimation;
	std::vector<Animation*> animations;

	int specialbar;
	int health;

	bool alive;
	bool crouch;
	bool jumping;
	bool attacking;

	Timer timer;
	


};

#endif // __KNIGHT_H_DEFINED__