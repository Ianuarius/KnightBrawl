/**
 * BaseState.h
 *
 */

#ifndef __BASESTATE_H_DEFINED__
#define __BASESTATE_H_DEFINED__

#include "Window.h"

#define STATE_GAME		0
#define STATE_MENU		1
#define STATE_VICTORY	2
#define STATE_CONTINUE	3
#define STATE_QUIT		4

const int LARGE_WIDTH =			1600;
const int LARGE_HEIGHT =		960;
const int SMALL_WIDTH =			1000;
const int SMALL_HEIGHT =		600;

const int SCREEN_WIDTH =		SMALL_WIDTH;
const int SCREEN_HEIGHT =		SMALL_HEIGHT;
const int RESOLUTION_WIDTH =	600;
const int RESOLUTION_HEIGHT =	360;

struct stateStatus 
{
	int status;
	bool prepend;
};

class BaseState 
{
	public:
		BaseState() {};
		~BaseState() {};
		virtual stateStatus update() = 0;
		virtual void render() = 0;
};

#endif // __BASESTATE_H_DEFINED__