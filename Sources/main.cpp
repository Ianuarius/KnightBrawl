/**
 * main.cpp
 *
 */

#include <SDL.h>
#include <stdio.h>
#include "Window.h"
#include "Input.h"
#include "SDL.h"
#include "Sprite.h"
#include "Level.h"

const int SCREEN_WIDTH = 640;
const int SCREEN_HEIGHT = 480;

int main(int argc, char* args[])
{
	SDL::init();

	Window window(SCREEN_WIDTH, SCREEN_HEIGHT);
	Camera camera(SCREEN_WIDTH, SCREEN_HEIGHT);
	
	SDL_Rect sama = {16, 16, 160, 160};
	SDL_Rect player = {0, 0, 0, 0};

	Level testi(&window, &camera);
	testi.load("test.tmx");
	camera.update(0, 0);
	bool gameover = false;
	float lerp = 0.1f;
	

	while (!gameover) {
		if (Input::keyState(SDL_SCANCODE_ESCAPE)) {
			gameover = true;
		}

		if (Input::keyState(SDL_SCANCODE_W)) {
			player.y -= 10;
		}
		
		if (Input::keyState(SDL_SCANCODE_S)) {
			player.y += 10;
		}
		
		if (Input::keyState(SDL_SCANCODE_A)) {
			player.x -= 10;
		}
		
		if (Input::keyState(SDL_SCANCODE_D)) {
			player.x += 10;
		}

		camera.update(camera.frame.x + (player.x - camera.frame.x) * lerp, 
					  camera.frame.y + (player.y - camera.frame.y) * lerp);

		Input::update();
		testi.render();
		window.refresh();
	}

	return 0;
}