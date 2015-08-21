/**
 * MenuState.cpp
 *
 */

#include "MenuState.h"

MenuState::MenuState(Window *window, Input *mainInput):
	window(window),
	mainInput(mainInput),
	splash(true),
	menu(window, "../Graphics/GUI/titlescreen.png"),
	splash_texture(window, "../Graphics/GUI/splash_screen.png"),
	boxing_selector_idle(window, "../Graphics/Knights/boxingknight/boxingknight_idle.png",
	50, 50, 0, 2, 8),
	boxing_selector_hit(window, "../Graphics/Knights/boxingknight/boxingknight_attack.png",
	50, 50, 0, 2, 8),
	state(0),
	font(new Font("../Fonts/Coval.ttf", 9)),
	version(new Text(font, Color("white"))),
	version_number("v0.3b")
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
		if (splash) {
			splash = false;
		} else {
			switch (state) {
			case START:
				status.status = STATE_C_SELECT;
				break;
			case OPTIONS:
				break;
			case QUIT:
				status.status = STATE_QUIT;
				break;
			default:
				break;
			}
		}
	}	
		
	if(mainInput->keyPressed(SDL_SCANCODE_LEFT)) {
		if (state > 0) {
			state--;
		}
	}
		
	if(mainInput->keyPressed(SDL_SCANCODE_RIGHT)) {
		if (state < 2) {
			state++;
		}
	}

	if(mainInput->keyPressed(SDL_SCANCODE_ESCAPE)) {
		if (!splash) {
			status.status = STATE_QUIT;
		}
	}

	return status;
}

void MenuState::render() 
{
	if (splash) {
		splash_texture.render(0, 0);
	} else {
		menu.render(0, 0);
		boxing_selector_idle.play(INFINITE_LOOP);
		
		switch (state) {
		case START:
			boxing_selector_idle.render(100 + 0, 167);
			break;
		case OPTIONS:
			boxing_selector_idle.render(100 + 154, 167);
			break;
		case QUIT:
			boxing_selector_idle.render(100 + 280, 167);
			break;
		default:
			boxing_selector_idle.render(100 + 0, 168);
			break;
		}

		version->print(window, version_number, 550, 340);
	}
}

StateData *MenuState::getStateData()
{
	return nullptr;
}