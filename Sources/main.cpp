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
	// Initialize the SDL and its augmentations.
	SDL::init();

	// Hide the cursor in the game window.
	SDL_ShowCursor(SDL_DISABLE);

	// Create the main window object where the game is rendered.
	Window window(SCREEN_WIDTH, 
				  SCREEN_HEIGHT, 
				  RESOLUTION_WIDTH, 
				  RESOLUTION_HEIGHT);

	// Create the main input that takes input in all states.
	Input mainInput;

	// Create and start the state manager.
	StateManager manager(&window, &mainInput);
	manager.run();
	
	// When the state manager is done running, clean up the SDL stuff.
	SDL::exit();

	return 0;
}