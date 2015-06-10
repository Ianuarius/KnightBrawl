/**
 * VictoryState.cpp
 *
 */

#include "VictoryState.h"

VictoryState::VictoryState(Window *window):
	window(window),
	victory(window, "victory_ph.png")
{	
	timer.start();
}

VictoryState::~VictoryState() 
{
}

stateStatus VictoryState::update()
{
	stateStatus status;
	status.status = STATE_CONTINUE;
	status.prepend = false;
	
	
	

	if (timer.getTicks() >= SLIDE_TIME){
		status.status = STATE_MENU;
		return status;
	}

	return status;
}

void VictoryState::render() 
{
	victory.render(0,0);
	//backgrounds[slide_index]->render(0, 0);
}