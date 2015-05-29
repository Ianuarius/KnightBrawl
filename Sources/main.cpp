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
#include "Level.h"
#include "PlayerActor.h"
#include "PlayerController.h"

const int SCREEN_WIDTH = 640;
const int SCREEN_HEIGHT = 480;

int main(int argc, char* args[])
{
	SDL::init();

	Window window(SCREEN_WIDTH, SCREEN_HEIGHT);
	Camera camera(SCREEN_WIDTH, SCREEN_HEIGHT);
	
	SDL_Rect sama = {16, 16, 160, 160};
	SDL_Rect player = {0, 0, 0, 0};
	SDL_Point start_point = {0, 0};

	std::vector<PlayerActor> players;

	PlayerActor player1(start_point);
	players.push_back(player1);
	PlayerController playerController1(&player1);

	Level testi(&window, &camera);
	testi.load("test.tmx");
	camera.lock(&player1);

	bool gameover = false;
	
	while (!gameover) {
		
		if (Input::keyState(SDL_SCANCODE_ESCAPE)) {
			gameover = true;
		}
		
		camera.update();

		playerController1.update();
		Input::update();
		testi.render();
		window.refresh();
	}

	return 0;
}