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
#include "Camera.h"
#include <stdio.h>
#include <vector>
#include "SDL.h"
#include "Sprite.h"
#include "PlayerActor.h"
#include "PlayerController.h"



class GameState : public BaseState
{
public:
	GameState(Window *window);
	~GameState();

	stateStatus update();
	void render();

private:
	Window *window;
	Level *testi;
	Texture *background;
	Font *font;
	Camera *camera;
	PlayerController *playerController1;
	bool gameover;
	

};

#endif // __GAMESTATE_H_DEFINED__d