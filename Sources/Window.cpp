/**
 * Window.cpp
 *
 */

#include "Window.h"

Window::Window(int window_width, int window_height):
	window(nullptr),
	renderer(nullptr),
	gScreenSurface(NULL),
	gHelloWorld(NULL)
{
	//Load splash image
	gHelloWorld = SDL_LoadBMP("testi.png");

	window = SDL_CreateWindow( "SDL Tutorial", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, window_width, window_height, SDL_WINDOW_SHOWN );
	renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
	
	gScreenSurface = SDL_GetWindowSurface(window);

	SDL_BlitSurface(gHelloWorld, NULL, gScreenSurface, NULL);
			
	//Update the surface
	SDL_UpdateWindowSurface(window);

}

void Window::refresh() {

	SDL_RenderPresent(renderer);

}

SDL_Renderer *Window::getRenderer() {
	return renderer;
}