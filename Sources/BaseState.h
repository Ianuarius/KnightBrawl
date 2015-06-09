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

const int SCREEN_WIDTH = 800;
const int SCREEN_HEIGHT = 480;
const int RESOLUTION_WIDTH = 400;
const int RESOLUTION_HEIGHT = 240;

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

	protected:
};

#endif // __BASESTATE_H_DEFINED__