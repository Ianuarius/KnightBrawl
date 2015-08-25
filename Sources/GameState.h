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
#include "Music.h"
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
	~GameState();
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
	Camera *camera;
	Font *font;
	Input *mainInput;
	Level *level;
	Music ost;
	StateData *stateData;
	Texture *background;
	Timer timer;
	Window *window;

	void executeMoves(int knight, int move);
	void checkHits(SDL_Rect *attacking_hb, int attacker, int damage, int direction);
	
	int players;
	int victory_counter;
	std::vector<Knight *> knights;
	std::vector<Projectile> projectiles;
	std::vector<PlayerActor *> playerActors;
	std::vector<PlayerController *> playerControllers;
	std::vector<SDL_Point> startPoints;
	bool gameover;
	Texture coin;
	std::vector<Text *> playernames;
};

#endif // __GAMESTATE_H_DEFINED__