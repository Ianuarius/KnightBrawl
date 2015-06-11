/**
 * GameState.cpp
 *
 */

#include "GameState.h"

GameState::GameState(Window *window):
	window(window),
	camera(nullptr),
	level(nullptr)
{
	timer.start();
	

	SDL_Rect sama = {16, 16, 160, 160};
	SDL_Rect player = {0, 0, 0, 0};
	SDL_Point start_point = {140, 0};
	
	for (int i = 0; i < PLAYERS; i++) {
		startPoints.push_back(start_point);
		playerControllers.push_back(new PlayerController(startPoints[i]));
	}
	
	camera = new Camera(RESOLUTION_WIDTH, RESOLUTION_HEIGHT, playerControllers[0]);

	for (int i = 0; i < PLAYERS; i++) {
		playerActors.push_back(new PlayerActor(window, camera, playerControllers[i]));
	}

	level = new Level(window, camera);
	level->load("test.tmx");
}

stateStatus GameState::update()
{
	stateStatus status;
	status.status = STATE_CONTINUE;
	status.prepend = false;

	if (Input::keyState(SDL_SCANCODE_ESCAPE)) {
		status.status = STATE_QUIT;
	}
		
	for (int i = 0; i < PLAYERS; i++) {
		playerControllers[i]->update();
	}

	Input::update();
	camera->update();

	return status;
}

void GameState::render()
{
	level->render();

	for (int i = 0; i < PLAYERS; i++) {
		playerActors[i]->render();
	}
}