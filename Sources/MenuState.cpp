/**
 * MenuState.cpp
 *
 */

#include "MenuState.h"

MenuState::MenuState(Window *window, Input *mainInput):
	window(window),
	mainInput(mainInput),
	menu(window, "Graphics/menu_ph.png")
{		
}

void MenuState::load(StateData *data)
{
	
}
stateStatus MenuState::update() 
{
	stateStatus status;
	status.status = STATE_CONTINUE;
	status.prepend = false;

		if (mainInput->keyPressed(SDL_SCANCODE_RETURN))	{
			status.status = STATE_C_SELECT;
		}	

		if(mainInput->keyPressed(SDL_SCANCODE_ESCAPE)) {
			status.status = STATE_QUIT;
		}

	return status;
}

void MenuState::render() 
{
	menu.render(0,0);
}

StateData *MenuState::getStateData()
{
	return nullptr;
}
