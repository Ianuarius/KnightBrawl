/**
 * Camera.cpp
 *
 */

#include "Camera.h"

Camera::Camera(int width, int height, PlayerController *playerController):
	playerController(playerController),
	camera_state(RIGHT)
{
	SDL_Rect realcamera = {0, 0, width, height};
	frame = realcamera;
}

/*
UPDATE CAMERA
- If player is in area FL < X < FR
- If player is in X < PL
- If player is in X > PR

DON'T UPDATE
- If player is in area PL < X < FL
- If player is in area FR < X < PR

*/

void Camera::update()
{
	// facing = playerController->getDirection();

	// NOTE(juha): The camera has states between which is alters as the player
	// moves on the screen. We'll check the player position based only on
	// the camera state.
	
	// NOTE(juha): IF player moves past the 40% mark right then the camera is moved
	// with him. Same thing if they move past 60% left

	// NOTE(juha): The position where the player actor is rendered on the screen
	// is handled in the PlayerActor.cpp.
	if (camera_state == RIGHT) {
		
		if (playerController->getLocation().x < 
			(frame.x + (frame.w * PAN_TR_L))) {
			
			camera_state = LEFT;
			frame.x = playerController->getLocation().x - (frame.w * FACING_R);

		} else if (playerController->getLocation().x > 
			(frame.x + (frame.w * FACING_L))) {

			frame.x = frame.x + playerController->getLocation().x - 
				(frame.x + (frame.w * FACING_L));
		}

	} else {
		
		if (playerController->getLocation().x > 
			(frame.x + (frame.w * PAN_TR_R))) {
			
			camera_state = RIGHT;
			frame.x = playerController->getLocation().x - (frame.w * FACING_L);

		} else if (playerController->getLocation().x < 
			(frame.x + (frame.w * FACING_R))) {

			frame.x = frame.x + playerController->getLocation().x - 
				(frame.x + (frame.w * FACING_R));
		}
	}

	frame.y = lerp(frame.y, playerController->getLocation().y);
	//frame.y = frame.y + (playerController->getLocation().y - frame.y) * LERP;
}

int Camera::lerp(int a, int b)
{
	return int(a + (b - a) * LERP);
}

int Camera::getState()
{
	return camera_state;
}

SDL_Rect Camera::getFrame()
{
	return frame;
}