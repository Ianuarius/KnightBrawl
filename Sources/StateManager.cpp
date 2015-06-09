/**
 * StateManager.cpp
 * 
 *
 */
#include "StateManager.h"

StateManager::StateManager(Window *window):
	window(window),
	glove(true)
{
		pushState(new MenuState(window));
}

StateManager::~StateManager() 
{

}

void StateManager::pushState(BaseState *state) 
{
	states.push_back(state);
}

void StateManager::popState() 
{
	states.pop_back();
}

void StateManager::run() 
{	
	while (glove) {
		BaseState *currentState = states.back();
		stateStatus new_status;
			
		Input::update();
		
		new_status = currentState->update();

	if (new_status.status != STATE_CONTINUE && !new_status.prepend) {
			popState();
		}

		switch (new_status.status)
		{
			case STATE_MENU:
				pushState(new MenuState(window));
				break;

			case STATE_GAME:
				pushState(new GameState(window));
				break;

			case STATE_QUIT:
				glove = false;
				break;
	
			default:
				break;
		}

		if (Input::keyPressed(SDL_SCANCODE_BACKSPACE)) {
			if (states.size() > 1) {
				popState();
			} else {
				glove = false;
			}
		}
		window->clear();
		currentState->render();
		window->refresh();
	}
}

	
