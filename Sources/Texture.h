/**
 * Texture.h
 *
 */

#ifndef __TEXTURE_H_DEFINED__
#define __TEXTURE_H_DEFINED__
#include <string>
#include "Window.h"


class Texture {

public:
	Texture(Window *window, std::string filename);
	void render();
	void free();
	
private:
	SDL_Texture *loadImage(std::string path);
	SDL_Renderer *renderer;
	SDL_Texture *texture;
	SDL_Rect clipRect;
};


#endif //__TEXTURE_H_DEFINED__