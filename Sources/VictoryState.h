/**
 * VictoryState.h
 *
 */

#ifndef __VICTORYSTATE_H_DEFINED__
#define __VICTORYSTATE_H_DEFINED__

#include "Window.h"
#include "Input.h"
#include "Font.h"
#include "BaseState.h"
#include "Text.h"
#include "Color.h"
#include "Texture.h"

class VictoryState : public BaseState 
{

public:
	VictoryState(Window *window);
	~VictoryState();

	void render();

private:
	Window *window;
	Text slideShowText();
	int slide_index;
	std::vector<Texture *> backgrounds;
	std::vector<std::string> texts;

	

};

#endif // __VICTORYSTATE_H_DEFINED__