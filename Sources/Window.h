/**
 * Window.h
 *
 */

#ifndef __WINDOW_H_DEFINED__
#define __WINDOW_H_DEFINED__

#include "SDL.h"
#include <stdio.h>
#include "Input.h"
#include "Timer.h"
#include "Color.h"

#define FRAMERATE 60
#define FPS_INTERVAL 1000

class Window 
{
public:
	Window(int window_width, int window_height, int resolution_width, int resolution_height);
	
	SDL_Renderer *getRenderer();
	Uint32 getDelta();
	
	void destroy();
	void resize(int window_width, int window_height, bool fullscreen);
	void refresh();
	void clear();
	void fill(Color color);
	void drawRect(int X, int Y, int W, int H, Color color);

	unsigned int window_width, logical_width;
	unsigned int window_height, logical_height;
	unsigned int originalWidth;
	unsigned int originalHeight;
private:

	SDL_Surface* gScreenSurface;
	SDL_Surface* gHelloWorld;
	SDL_Window *window;
	SDL_Renderer *renderer;

	Uint32 framerate, frame_delay, current_delta;

	bool fullscreen;

};


#endif //__WINDOW_H_DEFINED__