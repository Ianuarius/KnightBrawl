/**
 * GameState.cpp
 *
 */

#include "GameState.h"

GameState::GameState(Window *window):
	window(window),
	playerController1(nullptr),
	camera(nullptr),
	testi(nullptr)

{
	//Window window(SCREEN_WIDTH, SCREEN_HEIGHT, RESOLUTION_WIDTH, RESOLUTION_HEIGHT);
	timer.start();

}

GameState::~GameState()
{
}

stateStatus GameState::update()
{

	stateStatus status;
	status.status = STATE_CONTINUE;
	status.prepend = false;

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

	Level testi(window, &camera);
	testi.load("test.tmx");

	bool gameover = false;
	
	while (!gameover) {
		
		if (Input::keyState(SDL_SCANCODE_ESCAPE)) {
			gameover = true;
			status.status = STATE_QUIT;
		}
		
		Input::update();
		playerController1.update();
		camera.update();
		window->clear();
		testi.render();
		player1.render();
		window->refresh();
	}


	return status;
}

void GameState::render()
{

}