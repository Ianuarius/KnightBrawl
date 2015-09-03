/**
 * PlayerActor.h
 *
 * Controls the knight animations and sounds.
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
	// Parameters:
		// window			Pointer to the Window object created in main.cpp.
		// camera			Pointer to the Camera object created in GameState.
		// playerController	Pointer to the controller linked to the actor.
		// knight			Pointer to the knight the player is playing.
	PlayerActor(Window *window,
			    Camera *camera,
				PlayerController *playerController,
				Knight *knight);
	void render();

	// Updates the sound effects that need playing.
	void updateSound();
	
	// Updates the current animation that is playing.
	void updateAnimation();

	int facing_direction;

private:
	Animation *currentAnimation;
	Camera *camera;
	Knight *knight;
	PlayerController *playerController;
	SFX sfx_jump, sfx_land;
	Window *window;

	std::vector<SpecialCombo> *moves;
};

#endif //PLAYERACTOR_H_DEFINED