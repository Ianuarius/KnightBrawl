/**
 * main.cpp
 *
 */

#include <SDL.h>
#include <stdio.h>
#include <vector>
#include "Window.h"
#include "Input.h"
#include "SDL.h"
#include "Sprite.h"
#include "BaseState.h"
#include "Level.h"
#include "Camera.h"
#include "PlayerActor.h"
#include "PlayerController.h"
#include "StateManager.h"




int main(int argc, char* args[])
{
	SDL::init();

	Window window(SCREEN_WIDTH, SCREEN_HEIGHT, RESOLUTION_WIDTH, RESOLUTION_HEIGHT);
	
	StateManager manager(&window);
	manager.run();
	
	SDL::exit();

	return 0;
}

/*
int main(int argc, char* args[])
{
	SDL::init();

	Window window(SCREEN_WIDTH, SCREEN_HEIGHT, RESOLUTION_WIDTH, RESOLUTION_HEIGHT);
	
	SDL_Rect sama = {16, 16, 160, 160};
	SDL_Rect player = {0, 0, 0, 0};
	SDL_Point start_point = {140, 0};

	std::vector<PlayerActor> players;

	PlayerController playerController1(start_point);
	
	Camera camera(RESOLUTION_WIDTH, RESOLUTION_HEIGHT, &playerController1);
	PlayerActor player1(&window,
						&camera,
						&playerController1);
	players.push_back(player1);

	Level testi(&window, &camera);
	testi.load("test.tmx");

	bool gameover = false;
	
	while (!gameover) {
		
		if (Input::keyState(SDL_SCANCODE_ESCAPE)) {
			gameover = true;
		}
		
		Input::update();
		playerController1.update();
		camera.update();

		window.clear();
		testi.render();
		player1.render();
		window.refresh();
	}

	return 0;
}
*/