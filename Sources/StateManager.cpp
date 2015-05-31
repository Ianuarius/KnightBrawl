/**
 * StateManager.cpp
 * 
 *
 */
#include "StateManager.h"

StateManager::StateManager(Window *window) :
window(window),
glove(true),
music(nullptr) {
	pushState(new MenuState(window));
}

StateManager::~StateManager() {

}

void StateManager::pushState(BaseState *state) {
	states.push_back(state);
}

void StateManager::popState() {
	states.pop_back();
}

void StateManager::run() {
	music = new Music("");
	//music->play();

	while (glove) {
		BaseState *currentState = states.back();
		stateStatus new_status;

		Input::update();

		new_status = currentState->update();

		if (new_status.status != !new_status.prepend) {
			popState();
		}

		switch (new_status.status)
		{
	
		
		case STATE_GAME:
			pushState(new GameState(window));
			break;


		case STATE_MENU:
			pushState(new MenuState(window));
			break;

		

	
		

		case STATE_VICTORY:
			pushState(new VictoryState(window));
			break;

	

		default:
			break;
		}

		if (Input::keyPressed(SDL_SCANCODE_ESCAPE)) {
			if (states.size() > 1) {
				popState();
			}
			else {
				glove = false;
			}
		}

		window->clear();
		currentState->render();
		window->refresh();
	}
}