/**
 * PlayerActor.h
 *
 */

#ifndef PLAYERACTOR_H_DEFINED
#define PLAYERACTOR_H_DEFINED

#include "Camera.h"
#include "PlayerController.h"
#include "SDL.h"
#include "SFX.h"
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
	void updateSound();
	// std::vector<SpecialCombo> *special_combos;
	
private:
	Window *window;
	PlayerController *playerController;
	std::vector<SpecialCombo> *moves;
	Camera *camera;
	Knight *knight;
	Animation *currentAnimation;
	SFX sfx_jump, sfx_land;
};

#endif //PLAYERACTOR_H_DEFINED