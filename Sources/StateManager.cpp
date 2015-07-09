/**
 * StateManager.cpp
 * 
 *
 */
#include "StateManager.h"

StateManager::StateManager(Window *window, Input *mainInput):
	window(window),
	mainInput(mainInput),
	love(true)
{
		pushState(new MenuState(window, mainInput));
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
	while (love) {
		BaseState *currentState = states.back();
		stateStatus new_status;
			
		mainInput->update();
		new_status = currentState->update();

		if (new_status.status != STATE_CONTINUE && !new_status.prepend) {
				popState();
		}

		switch (new_status.status)
		{
			case STATE_MENU:
				pushState(new MenuState(window, mainInput));
				break;

			case STATE_GAME:
				pushState(new GameState(window, mainInput));
				break;
				
			case STATE_C_SELECT:
				pushState(new CharacterSelectState(window, mainInput));
				break;

			case STATE_QUIT:
				love = false;
				break;
	
			default:
				break;
		}

		if (mainInput->keyPressed(SDL_SCANCODE_BACKSPACE)) {
			if (states.size() > 1) {
				popState();
			} else {
				love = false;
			}
		}

		window->clear();
		currentState->render();
		window->refresh();
	}
}

	
