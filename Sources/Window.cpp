/**
 * Window.cpp
 *
 */

#include "Window.h"

Window::Window(int window_width, int window_height):
	window(nullptr),
	renderer(nullptr)
{

	window = SDL_CreateWindow("KnightBrawl", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, window_width, window_height, SDL_WINDOW_SHOWN);
	renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
	
	SDL_UpdateWindowSurface(window);

}

void Window::refresh()
{

	SDL_RenderPresent(renderer);

}

void Window::destroy()
{
	if (this->renderer)
	{
		SDL_DestroyRenderer(this->renderer);
		this->renderer =nullptr;
	}

	if (this->window)
	{
		SDL_DestroyWindow(this->window);
		this->window = nullptr;
	}
}

void Window::resize(int window_width, int window_height, bool fullscreen)
{
	destroy();

	Uint32 window_flag;

	this->fullscreen = fullscreen;

	if (fullscreen)
	{
		window_flag = SDL_WINDOW_FULLSCREEN_DESKTOP;
	}
	else
	{
		window_flag = SDL_WINDOW_SHOWN;
	}

	/*if (!window || !renderer)
	{
		printf("Oops! \n");
		printf( "SDL Error: %s \n", SDL_GetError());

	}
	*/

	window = SDL_CreateWindow("KnightBrawl", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, window_width, window_height, window_flag);
	renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);

	SDL_SetHint(SDL_HINT_RENDER_SCALE_QUALITY, "nearest");
	SDL_RenderSetLogicalSize(renderer, logical_width, logical_height);
	SDL_SetHint(SDL_HINT_RENDER_VSYNC, "1");

	this->window_width = window_width;
	this->window_height = window_height;

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