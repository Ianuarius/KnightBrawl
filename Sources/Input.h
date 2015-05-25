/**
 * Input.h
 *
 */

#ifndef __INPUT_H_DEFINED__
#define __INPUT_H_DEFINED__

#define KEYBOARD_SIZE 282
#include "SDL.h"

class Input {

public:
	Input();
	static bool keyState(int key);
	static void update();

private:
	static bool key[KEYBOARD_SIZE];
	static const Uint8* keyboard;

};


#endif //__INPUT_H_DEFINED__