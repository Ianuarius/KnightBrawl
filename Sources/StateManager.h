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
#include "ScoreState.h"
#include "MenuState.h"
#include "VictoryState.h"




class StateManager
{
public:
	StateManager(Window *window);
	~StateManager();

	void pushState (BaseState *state);
	void popState();

	void run();

protected:
	Window *window;
	Music *music;
	std::vector<BaseState*> states;
	bool glove;





}














#endif // __STATEMANAGER_H_DEFINED__