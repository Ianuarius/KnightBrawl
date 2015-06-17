/*
 * Rectangle.h
 *
 */

#ifndef __RECTANGLE_H_DEFINED__
#define __RECTANGLE_H_DEFINED__

#include "SDL.h"

class Rectangle
{
	public:
		Rectangle(int x, int y, int w, int h);
		Rectangle(SDL_Rect rect);
		~Rectangle();

		/* Can be casted to SDL_Rect like;
		 *		Rectangle rect(0, 0, 10, 10);
		 *		SDL_Rect test;
		 *
		 *		test = (SDL_Rect)rect;
		 */

		operator SDL_Rect() {
			SDL_Rect r = {x, y, w, h};
			return r;
		}

		SDL_Point Center();		// Returns X/Y of center of the rect

		SDL_Point TopLeft();	 // Returns X/Y of top left
		SDL_Point TopRight();	 // Returns X/Y of top right
		SDL_Point BottomLeft();	 // Returns X/Y of bottom left
		SDL_Point BottomRight(); // Returns X/Y of bottom right

		int x, y, w, h;
};

#endif //__RECTANGLE_H_DEFINED__