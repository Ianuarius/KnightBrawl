/**
 *	Rectangle.cpp
 *
 */
#include "Rectangle.h"

Rectangle::Rectangle(int x, int y, int w, int h):
	x(x),
	y(y),
	w(w),
	h(h) {
	// void
}

Rectangle::Rectangle(SDL_Rect rect) {
	if (!SDL_RectEmpty(&rect)) {
		x = rect.x;
		y = rect.y;
		w = rect.w;
		h = rect.h;
	}
}

Rectangle::~Rectangle() {}

SDL_Point Rectangle::Center() {
	SDL_Point p;
	p.x = x + (w/2);
	p.y = y + (h/2);

	return p;
}

SDL_Point Rectangle::TopLeft() {
	SDL_Point p;
	p.x = x;
	p.y = y;

	return p;
}

SDL_Point Rectangle::TopRight() {
	SDL_Point p;
	p.x = x + w;
	p.y = y;

	return p;
}

SDL_Point Rectangle::BottomLeft() {
	SDL_Point p;
	p.x = x;
	p.y = y + h;

	return p;
}

SDL_Point Rectangle::BottomRight() {
	SDL_Point p;
	p.x = x + w;
	p.y = y + h;

	return p;
}