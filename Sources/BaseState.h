/**
 * BaseState.h
 *
 * Abstract base class for the different game states.
 *
 */

#ifndef BASESTATE_H_DEFINED
#define BASESTATE_H_DEFINED

#include "StateData.h"
#include "Window.h"

#define STATE_GAME		0
#define STATE_MENU		1
#define STATE_VICTORY	2
#define STATE_CONTINUE	3
#define STATE_QUIT		4
#define STATE_C_SELECT	5

// NOTE(juha): Resolution currently for 10:6 aspect ratio due to the resolution
// of the test computer.
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

	// Updates the state of the current game state and returns a flag
	// to the state machine whether to continue or to change the state.
	virtual stateStatus update() = 0;

	// Renders the contents of the state.
	virtual void render() = 0;

	// Returns a StateData object that contains various types of information 
	// that needs to move between game states (e.g. the knights the players 
	// have selected).
	virtual StateData *getStateData() = 0;

	// Loads the StateData object from the previous state.
	virtual void load(StateData *data) = 0;
};

#endif // BASESTATE_H_DEFINED