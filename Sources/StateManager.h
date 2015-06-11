/**
 * StateManager.h
 * 
 *
 */

#ifndef __STATEMANAGER_H_DEFINED__
#define __STATEMANAGER_H_DEFINED__

#include <vector>
#include "BaseState.h"
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
	StateManager(Window *window);
	void run();
	void pushState(BaseState *state);
	void popState();

protected:
	Window *window;
	std::vector<BaseState*> states;
	bool glove;
};

#endif // __STATEMANAGER_H_DEFINED__