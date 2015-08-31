/**
 * StateManager.h
 * 
 * Handles the management of different game states
 * such as: GameState and MenuState.
 *
 */

#ifndef STATEMANAGER_H_DEFINED
#define STATEMANAGER_H_DEFINED

#include <vector>
#include "BaseState.h"
#include "CharacterSelectState.h"
#include "GameState.h"
#include "Input.h"
#include "MenuState.h"
#include "Music.h"
#include "SDL.h"
#include "VictoryState.h"
#include "Window.h"

class StateManager
{
public:
	// Parameters:
		// window		Pointer to the Window object created in main.cpp.
		// mainInput	Pointer to the Input object created in main.cpp.
	StateManager(Window *window, Input *mainInput);

	// When a new state is created, it's pushed into a states vector.
	void pushState(BaseState *state);

	// When a state shuts, it's popped from the states vector.
	void popState();

	// Runs the state machine and includes, essentially, the game loop.
	// (Additional comments omitted for teaching purposes. Ha!)
	void run();

protected:
	Input *mainInput;
	StateData *stateData;
	Window *window;
	std::vector<BaseState*> states;

	// True as long as the game is running.
	bool love;
};

#endif // STATEMANAGER_H_DEFINED