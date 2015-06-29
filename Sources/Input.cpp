/*
 * Input.cpp
 *
 */

#include "Input.h"

bool key[KEYBOARD_SIZE] = {0};
bool keyDown[KEYBOARD_SIZE] = {0};
bool keyUp[KEYBOARD_SIZE] = {0};
bool isLocked = false;

const Uint8* keyboard = NULL;

Input::Input()
{	
}

void Input::update() {	
	
	for (int i = 0; i < KEYBOARD_SIZE; i++) {
		keyDown[i] = false;
		keyUp[i]   = false;
	}

	SDL_Event event;

	while(SDL_PollEvent(&event)) {
		switch(event.type) {
			case SDL_KEYDOWN: {
					keyboard = SDL_GetKeyboardState(nullptr);

					int scancode = event.key.keysym.scancode;
					key[scancode] = true;
					keyDown[scancode] = true;
				}
				break;

			case SDL_KEYUP: {
					keyboard = SDL_GetKeyboardState(nullptr);

					int scancode = event.key.keysym.scancode;
					key[scancode] = false;
					keyUp[scancode] = true;
				}
				break;

			default:
				break;
		}
	}
}

bool Input::keyState(int k) {
	if(isLocked) {
		return false;
	}

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

bool Input::isKeyPressed(int k)
{
	if (isLocked) {
		return false;
	}

	if (!(keyboard)) {
		return false;
	}
	
	int sdl_key = static_cast<int>(k);

	if (keyboard[sdl_key]) {
		return true;
	}
	
	return false;
}

bool Input::isKeyDown(int key)
{
	if (isLocked) return false;

	if (key < 0 || key >= KEYBOARD_SIZE)
		return false;

	return (keyDown[key]);
}

bool Input::isKeyUp(int key)
{
	if (isLocked) return false;

	if (key < 0 || key >= KEYBOARD_SIZE)
		return false;

	return (keyUp[key]);
}

bool Input::alt() {
	return keyState(SDL_SCANCODE_LALT) || keyState(SDL_SCANCODE_RALT);
}

bool Input::ctrl() {
	return keyState(SDL_SCANCODE_LCTRL) || keyState(SDL_SCANCODE_RCTRL);
}

bool Input::shift() {
	return keyState(SDL_SCANCODE_LSHIFT) || keyState(SDL_SCANCODE_RSHIFT);
}

void Input::lock() {
	isLocked = true;
}

void Input::unlock() {
	isLocked = false;
}
