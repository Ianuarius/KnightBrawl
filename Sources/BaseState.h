/**
 * BaseState.h
 *
 */

#ifndef __BASESTATE_H_DEFINED__
#define __BASESTATE_H_DEFINED__

#include "Window.h"

#define STATE_GAME		0

#define STATE_QUIT		1

#define STATE_MENU		2

#define STATE_GAME		3

#define STATE_VICTORY	4
#define STATE_GAMEOVER  5
#define STATE_SCORE		6
#define STATE_OPTIONS	7
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