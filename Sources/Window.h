/**
 * Window.h
 *
 */

#ifndef __WINDOW_H_DEFINED__
#define __WINDOW_H_DEFINED__
#include "SDL.h"
#include <stdio.h>
#include "Input.h"

class Window 
{
public:
	Window(int window_width, int window_height);
	void refresh();
	SDL_Renderer *getRenderer();
	Uint32 getDelta();
private:

	SDL_Surface* gScreenSurface;

	SDL_Surface* gHelloWorld;

	SDL_Window *window;
	SDL_Renderer *renderer;

	Uint32 framerate, frame_delay, current_delta;

};


#endif //__WINDOW_H_DEFINED__