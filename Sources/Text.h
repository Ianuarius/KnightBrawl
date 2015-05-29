/**
 * Text.h
 * 
 */

#ifndef __TEXT_H_DEFINED__
#define __TEXT_H_DEFINED__

#include <string>
#include "SDL.h"
#include "Window.h"
#include "Font.h"
#include "Color.h"

class Text {
	public:
		Text(Font *font, Color font_color);
		~Text();

		void setFont(Font *font);
		void setColor(Color font_color);
		void print(Window *window, std::string text, int x, int y);

	private:

		int x, y;
		SDL_Texture *texture;
		Font *font;
		Color color;
};

#endif // __TEXT_H_DEFINED__