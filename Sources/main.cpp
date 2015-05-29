/*This source code copyrighted by Lazy Foo' Productions (2004-2015)
and may not be redistributed without written permission.*/

//Using SDL and standard IO
#include <SDL.h>
#include <stdio.h>
#include "Window.h"
#include "Input.h"
#include "SDL.h"
#include "Sprite.h"
#include "Level.h"

//Screen dimension constants
const int SCREEN_WIDTH = 640;
const int SCREEN_HEIGHT = 480;

int main(int argc, char* args[])
{
	SDL::init();

	Window window(SCREEN_WIDTH, SCREEN_HEIGHT);
	
	SDL_Rect sama = 
	{
		16,16,160,160
	};



	/*
	Sprite kuva(&window, "testi.png", 16, 16);
	kuva.crop(sama);
	kuva.render(0,0);
	*/	
	Level testi(&window);
	testi.load("test.tmx");
	testi.render();
	bool gameover = false;

	while (!gameover) {
		if (Input::keyState(SDL_SCANCODE_ESCAPE)) {
			gameover = true;
		}

		Input::update();
		window.refresh();
	}

	return 0;
}