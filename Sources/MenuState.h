/**
 * MenuState.h
 *
 */

#ifndef __MENUSTATE_H_DEFINED__
#define __MENUSTATE_H_DEFINED__

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

#define START		0
#define OPTIONS		1
#define QUIT		2

class MenuState: public BaseState 
{
public:
	MenuState(Window *window, Input *mainInput);
	void render();
	stateStatus update();
	void load(StateData *data);
	StateData *getStateData();

private:
	Font *font;
	Text *version;
	std::string version_number;
	Texture menu;
	Texture splash_texture;
	Animation boxing_selector_idle;
	Animation boxing_selector_hit;
	Input *mainInput;
	Window *window;
	GameState *game;
	int state;
	bool start;
	bool gameover;
	bool splash;
	SFX sfx_select1, sfx_select2;
};

#endif // __MENUSTATE_H_DEFINED__