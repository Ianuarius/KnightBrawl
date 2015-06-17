/**
 * Level.h
 *
 */

#ifndef __LEVEL_H_DEFINED__
#define __LEVEL_H_DEFINED__

#include <vector>
#include "Camera.h"
#include "PugiXML.h"
#include "Sprite.h"
#include "Window.h"
#include "Rectangle.h"

class Level
{
public:
	Level(Window *window, Camera *camera);
	~Level();

	void load(std::string level_name);
	void render();
	int getTile(int x, int y);
	void collides(PlayerController *playerController);
	pugi::xml_node Layer;
	SDL_Point start_position;

private:
	Window *window;
	Camera *camera;
	
	pugi::xml_document levelDocument;
	pugi::xml_parse_result result;

	int levelWidth, levelHeight;
	int tileSize;
	
	Sprite *levelTileSheet;
	std::vector<std::vector<int> > TileData;
};

#endif //__LEVEL_H_DEFINED__