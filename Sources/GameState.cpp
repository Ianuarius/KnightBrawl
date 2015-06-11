/**
 * GameState.cpp
 *
 */

#include "GameState.h"

GameState::GameState(Window *window):
	window(window),
	playerController1(nullptr),
	camera(nullptr),
	level(nullptr)
{
	timer.start();

	SDL_Rect sama = {16, 16, 160, 160};
	SDL_Rect player = {0, 0, 0, 0};
	SDL_Point start_point = {140, 0};

	std::vector<PlayerActor> players;
	PlayerController playerController1(start_point);
	
	Camera camera(RESOLUTION_WIDTH, RESOLUTION_HEIGHT, &playerController1);
	PlayerActor player1(window,
						&camera,
						&playerController1);

	players.push_back(player1);

	level = new Level(window, &camera);
	level->load("test.tmx");

}

stateStatus GameState::update()
{
	stateStatus status;
	status.status = STATE_CONTINUE;
	status.prepend = false;

	bool gameover = false;
	
	while (!gameover) {
		
		if (Input::keyState(SDL_SCANCODE_ESCAPE)) {
			gameover = true;
			status.status = STATE_QUIT;
		}
		
		Input::update();
		playerController1.update();
		camera.update();
	}

	return status;
}

void GameState::render()
{
	level->render();
	player1.render();

}