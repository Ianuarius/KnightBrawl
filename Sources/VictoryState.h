/**
 * VictoryState.h
 *
 */

#ifndef __VICTORYSTATE_H_DEFINED__
#define __VICTORYSTATE_H_DEFINED__

#include "Window.h"
#include "Input.h"
#include "Font.h"
#include "BaseState.h"
#include "Text.h"
#include "Color.h"
#include "Texture.h"
#include "Timer.h"
#include "StateManager.h"

#define SLIDE_TIME 10000

class VictoryState : public BaseState 
{
public:
	VictoryState(Window *window);
	~VictoryState();
	stateStatus update();
	void render();

private:
	Window *window;
	Text slideShowText();
	Texture victory;
	int slide_index;
	Timer timer;
};

#endif // __VICTORYSTATE_H_DEFINED__