/**
 * VictoryState.cpp
 *
 */

#include "VictoryState.h"

VictoryState::VictoryState(Window *window):
	window(window)
{	
}

VictoryState::~VictoryState() 
{
}

void VictoryState::render() 
{
	backgrounds[slide_index]->render(0, 0);
}