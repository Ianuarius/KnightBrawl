/**
 * GameState.h
 *
 * State where the gameplay happens.
 *
 */

#ifndef GAMESTATE_H_DEFINED
#define GAMESTATE_H_DEFINED

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
	// Parameters:
		// window		Pointer to the Window object created in main.cpp.
		// mainInput	Pointer to the Input object created in main.cpp.
	GameState(Window *window, Input *mainInput);
	~GameState();
	stateStatus update();
	StateData *getStateData();
	void load(StateData *data);
	void render();

private:
	// If a player makes a move executeMoves activates all its functionality.
	void executeMoves(int knight, int move);

	// Checks if players were hit by anything.
	void checkHits(SDL_Rect *attacking_hb, int attacker, 
				   Entity *weapon, int damage, int direction);

	Camera		*camera;
	Font		*font;
	Input		*mainInput;
	Level		*level;
	Music		ost;
	StateData	*stateData;
	Texture		*background;
	Texture		coin;
	Timer		timer;
	Window		*window;

	std::vector<Knight *>				knights;
	std::vector<Projectile>				projectiles;
	std::vector<PlayerActor *>			playerActors;
	std::vector<PlayerController *>		playerControllers;
	std::vector<SDL_Point>				startPoints;
	std::vector<Text *>					playernames;

	bool gameover;
	int players;
	int victory_counter;
};

#endif // GAMESTATE_H_DEFINED