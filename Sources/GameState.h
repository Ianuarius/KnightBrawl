/**
 * GameState.h
 *
 */

#ifndef __GAMESTATE_H_DEFINED__
#define __GAMESTATE_H_DEFINED__

#include <stdio.h>
#include <vector>
#include "Animation.h"
#include "BaseState.h"
#include "Camera.h"
#include "Color.h"
#include "Font.h"
#include "Level.h"
#include "Input.h"
#include "PlayerActor.h"
#include "PlayerController.h"
#include "SDL.h"
#include "Sprite.h"
#include "Text.h"
#include "Texture.h"
#include "Window.h"

#define PLAYERS	1

class GameState : public BaseState
{
public:
	GameState(Window *window);
	stateStatus update();
	void render();

private:
	Window *window;
	Level *level;
	Texture *background;
	Font *font;
	Camera *camera;
	Timer timer;
	
	std::vector<PlayerActor *> playerActors;
	std::vector<PlayerController *> playerControllers;
	std::vector<SDL_Point> startPoints;
	bool gameover;
};

#endif // __GAMESTATE_H_DEFINED__d