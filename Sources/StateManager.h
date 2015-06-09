/**
 * StateManager.h
 * 
 *
 */

#ifndef __STATEMANAGER_H_DEFINED__
#define __STATEMANAGER_H_DEFINED__

#include <vector>
#include "Window.h"
#include "Input.h"
#include "BaseState.h"
#include "Music.h"
#include "GameState.h"
#include "MenuState.h"
#include "VictoryState.h"
#include "SDL.h"

class StateManager
{
public:
	StateManager(Window *window);
	~StateManager();
	void run();
	void pushState(BaseState *state);
	void popState();

protected:
	Window *window;
	std::vector<BaseState*> states;
	bool glove;

	
};

#endif // __STATEMANAGER_H_DEFINED__