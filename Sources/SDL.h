/**
 * SDL.h
 *
 */

#ifndef __SDL_H_INCLUDED__
#define __SDL_H_INCLUDED__

#include <stdio.h>
#include <vector>

#if defined(WIN32) || defined(_WIN32) || defined(__WIN32) && !defined(__CYGWIN__)
	#include <SDL.h>
	#include <SDL_image.h>
	#include <SDL_ttf.h>
	#include <SDL_mixer.h>
	#include <SDL_net.h>
#else
	#include "SDL2/SDL.h"
	#include "SDL2/SDL_image.h"
	#include "SDL2/SDL_ttf.h"
	#include "SDL2/SDL_mixer.h"
	#include "SDL_net.h"
#endif


// High level wrapper for SDL2 calls

// Shuts up the compiler about unused parameters.
#define UNUSED(x) ((void)(x))

namespace SDL
{
	// Initializes SDL2, must be called before everything else
	bool init();

	// Destroys SDL2, must be called when program closes
	void exit();
};

#endif // __SDL_H_INCLUDED__
