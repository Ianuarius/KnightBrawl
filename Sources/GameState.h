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

class GameState : public BaseState
{
public:
	GameState(Window *window, Input *mainInput);
	stateStatus update();
	StateData *getStateData();
	void load(StateData *data);
	void render();
	enum knights {
		CACTUS_KNIGHT,
		WIZARD_KNIGHT,
		ROGUE_KNIGHT,
		LANTERN_KNIGHT,
		DRAGON_KNIGHT
	};

private:
	Window *window;
	Level *level;
	Texture *background;
	Font *font;
	Camera *camera;
	Timer timer;
	Input *mainInput;
	StateData *stateData;
	
	int players;
	std::vector<Knight *> knights;
	std::vector<PlayerActor *> playerActors;
	std::vector<PlayerController *> playerControllers;
	std::vector<SDL_Point> startPoints;
	bool gameover;
	Texture coin;
	std::vector<Text *> playernames;
};

#endif // __GAMESTATE_H_DEFINED__