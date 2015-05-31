/**
 * MenuState.cpp
 *
 */
#include "MenuState.h"
MenuState::MenuState(Window *window) :
window(window),
menu_x(170),
menu_y(110),
menu_index(0),
selector(window, ""),
font(nullptr) {
	font = new Font("fonts/AveriaSerif-Bold.ttf", 14);

	menu_items.push_back(new Text(font, Color("white")));
	menu_items.push_back(new Text(font, Color("white")));
	menu_items.push_back(new Text(font, Color("white")));
}

MenuState::~MenuState() {
	delete font;
}

stateStatus MenuState::update() {
	stateStatus status;

	if (Input::keyPressed(SDL_SCANCODE_UP) || Input::keyPressed(SDL_SCANCODE_W)) {
		if (menu_index > 0) {
			menu_index--;
		}
	}

	if (Input::keyPressed(SDL_SCANCODE_DOWN) || Input::keyPressed(SDL_SCANCODE_S)) {
		if (menu_index < menu_items.size() - 1) {
			menu_index++;
		}
	}

	if (Input::keyPressed(SDL_SCANCODE_RETURN) || Input::keyPressed(SDL_SCANCODE_SPACE)) {
		switch (menu_index)
		{
		case 0:
			status.status = STATE_GAME;
			break;


		case 1:
			status.status = STATE_QUIT;
			break;

		default:
			break;
		}
	}

	return status;
}

void MenuState::render() {
	selector.render(menu_x - 25, menu_y + menu_index * 20);
	menu_items[0]->print(window, "Start", menu_x, menu_y + 0);
	menu_items[1]->print(window, "Options", menu_x, menu_y + 20);
	menu_items[2]->print(window, "Quit", menu_x, menu_y + 40);
}