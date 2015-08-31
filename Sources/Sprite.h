/**
 * Sprite.h
 *
 * Handles the definition of sprite sheets that are also
 * used as a base for animations in Animation class.
 *
 * The given image file is cropped into sections of the given width and height
 * and then indexed from top left to bottom right.
 *
 */

#ifndef SPRITE_H_DEFINED
#define SPRITE_H_DEFINED

#include <vector>
#include "SDL.h"
#include "Texture.h"
#include "Window.h"

class Sprite : public Texture
{
public:
	// Parameters:
		// window		Pointer to the Window object created in main.cpp.
		// filename		The path and filename to the image file.
		// width		The width of a single sprite on the sprite sheet.
		// height		The height of a single sprite on the sprite sheet.
	Sprite(Window *window, std::string filename, int width, int height);

	// Sets the section that will be cropped and rendered 
	// from the sprite sheet. 
	void setIndex(int i);
	void render(int x, int y);

private:
	Window *window;
	int width;
	int height;		
	int index;
	std::vector<SDL_Rect> spriteSheet;

};

#endif // SPRITE_H_DEFINED