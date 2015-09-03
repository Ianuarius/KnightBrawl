/**
 * ScoreState.h
 *
 */

#ifndef __SCORESTATE_H_DEFINED__
#define __SCORESTATE_H_DEFINED__

#include "Window.h"
#include "Input.h"
#include "Font.h"
#include "BaseState.h"
#include "Text.h"
#include "Color.h"
#include "Texture.h"

class ScoreState {
	
public:
	ScoreState(Window *window);
	~ScoreState();
	stateStatus update();
	void render();

private:
	Window *window;
	Texture background;
	Font *font;
	int score;
	Text *playername;
};

#endif // __SCORESTATE_H_DEFINED__