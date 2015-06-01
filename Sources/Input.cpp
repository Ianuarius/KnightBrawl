/**
 * Input.cpp
 *
 */

#include "Input.h"

bool Input::key[KEYBOARD_SIZE] = {0};
const Uint8* Input::keyboard = NULL;

Input::Input()
{

}

void Input::update() {	
	SDL_Event event;

	while(SDL_PollEvent(&event)) {
		switch(event.type) {
			case SDL_KEYDOWN: {
					keyboard = SDL_GetKeyboardState(nullptr);

					int scancode = event.key.keysym.scancode;
					key[scancode] = true;
				}
				break;

			case SDL_KEYUP: {
					keyboard = SDL_GetKeyboardState(nullptr);

					int scancode = event.key.keysym.scancode;
					key[scancode] = false;
				}
				break;

			default:
				break;
		}
	}
}

bool Input::keyState(int k)
{
	/*
	if(isLocked) {
		return false;
	}
	*/
	if(k < 0 || k >= KEYBOARD_SIZE) {
		return false;
	}

	return key[k];
}

bool Input::keyPressed(int k) {
	bool state = keyState(k);
	key[k] = false;

	return state;
}