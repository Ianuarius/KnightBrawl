/**
 * MenuState.h
 *
 */

#ifndef __MENUSTATE_H_DEFINED__
#define __MENUSTATE_H_DEFINED__
#include "Window.h"
#include "Input.h"
#include "Font.h"
#include "BaseState.h"
#include "Text.h"
#include "Color.h"
#include "Texture.h"
#include "GameState.h"
#include <iostream>
#include <stdio.h>
#include "Music.h"


class MenuState: public BaseState 
{

public:
	MenuState(Window *window);
	~MenuState();
	void render();
	stateStatus update();
private:
	
	Texture menu;
	Window *window;
	GameState *game;
	bool start;
	bool gameover;
};

#endif // __MENUSTATE_H_DEFINED__