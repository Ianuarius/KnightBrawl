/**
 * Texture.cpp
 *
 */
#include "Texture.h"

Texture::Texture(Window *window, std::string filename) {
	renderer = window->getRenderer();
	texture = loadImage(filename);

}

SDL_Texture *Texture::loadImage(std::string path) {

	SDL_Surface* surface = IMG_Load(path.c_str());
	
	SDL_Texture *newTexture = SDL_CreateTextureFromSurface(renderer, surface);

	SDL_FreeSurface(surface);

	return newTexture;
}

void Texture::render() {
	int width, height;
	SDL_QueryTexture(texture, NULL, NULL, &width, &height);

	SDL_Rect clip = {0, 0, width, height};
	clipRect = clip;
	
	SDL_Rect destination = {0, 0, clipRect.w, clipRect.h};
	SDL_RendererFlip flag = SDL_FLIP_NONE;

	SDL_RenderCopyEx(renderer, texture, &clipRect, &destination, 0, NULL, flag);

}

void Texture::free()
{
	SDL_DestroyTexture(texture);
}