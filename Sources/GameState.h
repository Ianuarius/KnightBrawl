/**
 * GameState.h
 *
 */
#ifndef __GAMESTATE_H_DEFINED__
#define __GAMESTATE_H_DEFINED__

#include "Window.h"
#include "Input.h"
#include "Font.h"
#include "BaseState.h"
#include "Text.h"
#include "Color.h"
#include "Texture.h"
#include "Animation.h"
#include "Level.h"

class GameState : public BaseState
{
public:
	GameState(Window *window);
	~GameState();

	stateStatus update();
	void render();


private:
	Animation *test;
	Window *window;
	Level *level;
	Texture *background;
	Font *font;
	Text *tooltip, signText;
	std::string tooltip_s, signText_s;

	SDL_Rect hilight;

};

#endif // __GAMESTATE_H_DEFINED__