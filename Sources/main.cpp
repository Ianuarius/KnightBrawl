/**
 * main.cpp
 *
 */

#include "Input.h"
#include "SDL.h"
#include "StateManager.h"
#include "Window.h"

int main(int argc, char* args[])
{
	SDL::init();
	
	Window window(SCREEN_WIDTH, SCREEN_HEIGHT, RESOLUTION_WIDTH, RESOLUTION_HEIGHT);
	Input mainInput;
	StateManager manager(&window, &mainInput);
	manager.run();
	
	SDL::exit();

	return 0;
}