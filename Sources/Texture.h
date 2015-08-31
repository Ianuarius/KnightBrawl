/**
 * Texture.h
 *
 * Handles the definition of texture images that are also 
 * used as a base for sprite sheets in Sprite class.
 * 
 */

#ifndef TEXTURE_H_DEFINED
#define TEXTURE_H_DEFINED
#include <string>
#include "Window.h"


class Texture 
{
public:
	// Parameters:
		// window		Pointer to the Window object created in main.cpp.
		// filename		The path and filename to the image file.
	Texture(Window *window, std::string filename);
	void free();

	// Defines a section as an SDL_Rect that will be rendered of the image.
	void crop(SDL_Rect rect);
	int getWidth();
	int getHeight();
	void render(int x, int y);

	bool flip;
	
private:
	SDL_Texture *loadImage(std::string path);

	SDL_Renderer *renderer;
	SDL_Texture *texture;
	SDL_Rect clipRect;
	int height;
	int width;
};

#endif // TEXTURE_H_DEFINED