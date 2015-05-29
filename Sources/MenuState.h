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


class MenuState : public BaseState 
{

public:
	MenuState(Window *window);
	~MenuState();

	stateStatus update();
	void render();


private:
	Texture selector;
	int menu_index;

	std::vector<Text *> menu_items;
	Window *window;
	Font *font;
	int menu_x;
	int menu_y;


};



#endif // __MENUSTATE_H_DEFINED__