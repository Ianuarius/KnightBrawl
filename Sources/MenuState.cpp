/**
 * MenuState.cpp
 *
 */
#include "MenuState.h"
MenuState::MenuState(Window *window):
	window(window),
	menu(window, "menu_ph.png")
{		
}

MenuState::~MenuState() 
{
}

stateStatus MenuState::update() 
{

	stateStatus status;
	status.status = STATE_CONTINUE;
	status.prepend = false;


		if (Input::keyPressed(SDL_SCANCODE_RETURN))	{
			status.status = STATE_GAME;
		}	

		if(Input::keyPressed(SDL_SCANCODE_ESCAPE)) {
			status.status = STATE_QUIT;
		}

		/*if(Input::keyPressed(SDL_SCANCODE_CAPSLOCK)){
			status.status = STATE_VICTORY;
		}*/

			
	return status;
}

void MenuState::render() 
{
	menu.render(0,0);
	//menu_items[0]->print(window, "Start", menu_x, menu_y + 0);
}