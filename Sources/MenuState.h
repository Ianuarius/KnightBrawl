/**
 * MenuState.h
 *
 * State for the game's main menu.
 *
 * This state is where the splash screen is currently displayed and where the
 * players can choose to move to the character selection, options, or to quit
 * the game.
 *
 * The MenuState is operated only via the mainInput. The playerControllers are
 * not created until CharacterSelectState.
 */

#ifndef MENUSTATE_H_DEFINED
#define MENUSTATE_H_DEFINED

#include <stdio.h>
#include "BaseState.h"
#include "Color.h"
#include "Font.h"
#include "GameState.h"
#include "Input.h"
#include "SFX.h"
#include "Music.h"
#include "Text.h"
#include "Texture.h"

// NOTE(juha): Different menu selection options.
#define START		0
#define OPTIONS		1
#define QUIT		2

class MenuState: public BaseState 
{
public:
	// Parameters:
		// window		Pointer to the Window object created in main.cpp.
		// mainInput	Pointer to the Input object created in main.cpp.
	MenuState(Window *window, Input *mainInput);
	void load(StateData *data);
	stateStatus update();
	void render();
	StateData *getStateData();

private:
	Animation boxing_selector_idle;
	Animation boxing_selector_hit;
	Font *font;
	GameState *game;
	Input *mainInput;
	SFX sfx_select1, sfx_select2;
	Text *version;
	Texture menu;
	Texture splash_texture;
	Window *window;

	std::string version_number;

	int state;
	bool start;
	bool gameover;

	// MenuState displays different content and operates differently based on
	// whether the splash variable is true or false.
	bool splash;
};

#endif // MENUSTATE_H_DEFINED