/**
 * Sprite.h
 *
 */

#ifndef __SPRITE_H_DEFINED__
#define __SPRITE_H_DEFINED__
#include <vector>
#include "SDL.h"
#include "Window.h"
#include "Texture.h"


class Sprite : public Texture
{

public:
	Sprite(Window *window, std::string filename, int width, int height);
	void render(int x, int y);
	void setIndex(int i);
private:
	Window *window;
	int width;
	int height;		
	int index;
	std::vector<SDL_Rect> spriteSheet;

};

#endif //__SPRITE_H_DEFINED__