/**
 * Texture.h
 *
 */

#ifndef __TEXTURE_H_DEFINED__
#define __TEXTURE_H_DEFINED__
#include <string>
#include "Window.h"


class Texture 
{

public:
	Texture(Window *window, std::string filename);
	void render(int x, int y);
	void free();
	void crop(SDL_Rect rect);
	int getWidth();
	int getHeight();
	bool flip;
	
private:
	SDL_Texture *loadImage(std::string path);
	SDL_Renderer *renderer;
	SDL_Texture *texture;
	SDL_Rect clipRect;
	int height;
	int width;
};


#endif //__TEXTURE_H_DEFINED__