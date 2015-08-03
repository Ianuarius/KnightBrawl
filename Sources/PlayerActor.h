/**
 * PlayerActor.h
 *
 */

#ifndef __PLAYERACTOR_H_DEFINED__
#define __PLAYERACTOR_H_DEFINED__

#include "Camera.h"
#include "PlayerController.h"
#include "SDL.h"
#include "Texture.h"

class PlayerActor
{
public:
	PlayerActor(Window *window,
			    Camera *camera,
				PlayerController *playerController,
				Knight *knight);
	void render();
	int facing_direction;
	void updateAnimation();
	std::vector<SpecialCombo> *special_combos;
	
private:
	Window *window;
	PlayerController *playerController;
	std::vector<SpecialCombo> *moves;
	Camera *camera;
	Knight *knight;
	Animation *currentAnimation;
};

#endif //__PLAYERACTOR_H_DEFINED__