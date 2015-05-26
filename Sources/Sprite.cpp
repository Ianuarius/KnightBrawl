/**
 * Sprite.cpp
 *
 */
#include "Sprite.h"

Sprite::Sprite(Window *window, std::string filename, int spriteWidth, int spriteHeight):
	Texture(window, filename),
	index(0)
{
		int columns = getWidth() / spriteWidth;
		int rows = getHeight() / spriteHeight;
	
		for (int i = 0; i < rows; ++i)
		{
			for (int j = 0; j < columns; ++j)
			{
				SDL_Rect rect = {j*spriteWidth, i*spriteHeight, spriteWidth, spriteHeight};
				spriteSheet.push_back(rect);
			}
		}
}

void Sprite::setIndex(int i) {
	if (i < 0) {
		index = 0;
	} else {
		index = i;
	}
}

void Sprite::render(int x, int y)
{
	crop(spriteSheet[index]);
	Texture::render(x, y);
}