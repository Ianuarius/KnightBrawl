/*This source code copyrighted by Lazy Foo' Productions (2004-2015)
and may not be redistributed without written permission.*/

//Using SDL and standard IO
#include <SDL.h>
#include <stdio.h>
#include "Window.h"
#include "Input.h"
#include "SDL.h"
#include "Texture.h"

//Screen dimension constants
const int SCREEN_WIDTH = 640;
const int SCREEN_HEIGHT = 480;

int main( int argc, char* args[] )
{
	
	SDL::init();

	Window window(SCREEN_WIDTH, SCREEN_HEIGHT);
	
	Texture kuva(&window, "testi.png");
		kuva.render();

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