/**
 * MenuState.h
 *
 */

#ifndef __MENUSTATE_H_DEFINED__
#define __MENUSTATE_H_DEFINED__

#include <iostream>
#include <stdio.h>
#include "BaseState.h"
#include "Color.h"
#include "Font.h"
#include "GameState.h"
#include "Input.h"
#include "Music.h"
#include "Text.h"
#include "Texture.h"

class MenuState: public BaseState 
{
public:
	MenuState(Window *window, Input *mainInput);
	void render();
	stateStatus update();

private:
	Texture menu;
	Input *mainInput;
	Window *window;
	GameState *game;
	bool start;
	bool gameover;
};

#endif // __MENUSTATE_H_DEFINED__