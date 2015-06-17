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
	SDL_Point start_point = {140, 0};

	bool multiplayer = true;

	if (PLAYERS <= 1) {
		multiplayer = false;
	}
	
	for (int i = 0; i < PLAYERS; i++) {
		startPoints.push_back(start_point);
		playerControllers.push_back(new PlayerController(startPoints[i], multiplayer, i));
	}
	
	camera = new Camera(RESOLUTION_WIDTH, RESOLUTION_HEIGHT, playerControllers[0]);

	for (int i = 0; i < PLAYERS; i++) {
		playerActors.push_back(new PlayerActor(window, camera, playerControllers[i]));
	}
	
	level = new Level(window, camera);
	level->load("tavern_small.tmx");
	
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
	level->collides(playerControllers[0]);
	playerControllers[0]->old_bound = playerControllers[0]->boundbox;

	// Tallennetaan pelaajan tietoihin sen edellinen sijainti ja jos tapahtuu
	// collisioni, niin palautetaan pelaaja edelliseen sijaintiin.
	camera->update();

	printf("Player x: %d - Player y: %d\n", playerControllers[0]->boundbox.x, playerControllers[0]->boundbox.y);

	return status;
}

void GameState::render()
{
	level->render();

	for (int i = 0; i < PLAYERS; i++) {
		playerActors[i]->render();
	}
}