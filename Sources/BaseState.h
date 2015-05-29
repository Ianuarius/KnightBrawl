/**
 * BaseState.h
 *
 */

#ifndef __BASESTATE_H_DEFINED__
#define __BASESTATE_H_DEFINED__

#include "Window.h"

#define STATE_QUIT		1

#define STATE_MENU		2

#define STATE_GAME		4


#define STATE_VICTORY	6
#define STATE_GAMEOVER  7
#define STATE_SCORE		8

#define STATE_CREDITS	666

struct stateStatus {
	int status;
	bool prepend;
};

class BaseState {

	public:
		BaseState() {};
		~BaseState() {};

		virtual stateStatus update() = 0;
		virtual void render() = 0;

	protected:
};

#endif // __BASESTATE_H_DEFINED__