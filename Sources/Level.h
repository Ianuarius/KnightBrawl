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

class Level
{
public:
	Level(Window *window, Camera *camera);
	~Level();

	void load(std::string level_name);
	void render();
	pugi::xml_node Layer;

private:
	Window *window;
	Camera *camera;
	
	pugi::xml_document levelDocument;
	pugi::xml_parse_result result;

	int levelWidth, levelHeight;
	int tileSize;
	
	Sprite *levelTileSheet;
	std::vector<std::vector<int> > tileData;
};

#endif //__LEVEL_H_DEFINED__