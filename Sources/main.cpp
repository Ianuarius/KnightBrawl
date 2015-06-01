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
#include "Camera.h"
#include "PlayerActor.h"
#include "PlayerController.h"

const int SCREEN_WIDTH = 640;
const int SCREEN_HEIGHT = 480;

int main(int argc, char* args[])
{
	SDL::init();

	Window window(SCREEN_WIDTH, SCREEN_HEIGHT);
	
	SDL_Rect sama = {16, 16, 160, 160};
	SDL_Rect player = {0, 0, 0, 0};
	SDL_Point start_point = {340, 280};

	std::vector<PlayerActor> players;

	PlayerController playerController1;
	
	Camera camera(SCREEN_WIDTH, SCREEN_HEIGHT, &playerController1);
	PlayerActor player1(&window, 
						&camera, 
						&playerController1, 
						start_point);
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