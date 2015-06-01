/**
 * Window.cpp
 *
 */

#include "Window.h"

Window::Window(int window_width, int window_height):
	window(nullptr),
	renderer(nullptr)
{
	window = SDL_CreateWindow( "SDL Tutorial", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, window_width, window_height, SDL_WINDOW_SHOWN );
	renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
	
	SDL_UpdateWindowSurface(window);

}

void Window::refresh()
{

	SDL_RenderPresent(renderer);

}

SDL_Renderer *Window::getRenderer()
{
	return renderer;
}
/*
void Window::fill(Color color)
{
	SDL_SetRenderDrawColor(renderer,
						   color.r(),
						   color.g(),
						   color.b(),
						   color.a());

	SDL_RenderClear(renderer);
}


void Window::clear()
{
	fill(Color("black"));
}*/

Uint32 Window::getDelta()
{
	return current_delta;
}