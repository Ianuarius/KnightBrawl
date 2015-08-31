/**
 * Texture.cpp
 *
 */
#include "Texture.h"

Texture::Texture(Window *window, std::string filename):
	clipRect(),
	flip(false)
{
	renderer = window->getRenderer();
	texture = loadImage(filename);

	SDL_QueryTexture(texture, NULL, NULL, &width, &height);
}

void Texture::free()
{
	SDL_DestroyTexture(texture);
}

SDL_Texture *Texture::loadImage(std::string path)
{
	SDL_Surface* surface = IMG_Load(path.c_str());
	
	if (!surface) {
		printf("Failed to load texture %s.\n", path.c_str());
		return nullptr;
	}

	SDL_SetColorKey(surface, SDL_TRUE, SDL_MapRGB(surface->format,
												0xFF, 0, 0xFF));
	SDL_Texture *newTexture = SDL_CreateTextureFromSurface(renderer, surface);

	SDL_FreeSurface(surface);

	return newTexture;
}

void Texture::crop(SDL_Rect rect)
{
	clipRect.x = rect.x;
	clipRect.y = rect.y;
	clipRect.w = rect.w;
	clipRect.h = rect.h;
}

int Texture::getWidth()
{
	return width;
}

int Texture::getHeight()
{
	return height;
}

void Texture::render(int x, int y)
{	
	// NOTE(juha): If the clipRect hasn't been set in crop
	// then render the whole texture.
	if (clipRect.w <= 0 && clipRect.h <= 0) {
		int width, height;
		SDL_QueryTexture(texture, NULL, NULL, &width, &height);

		SDL_Rect clip = {0, 0, width, height};
		clipRect = clip;
	}

	SDL_Rect destination = {x, y, clipRect.w, clipRect.h};
	SDL_RendererFlip flag = SDL_FLIP_NONE;

	if (flip) {
		flag = SDL_FLIP_HORIZONTAL;
	}

	SDL_RenderCopyEx(renderer, texture, &clipRect, &destination, 0, NULL, flag);
}
