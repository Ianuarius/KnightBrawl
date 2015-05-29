/**
 * Level.h
 *
 */

#ifndef __LEVEL_H_DEFINED__
#define __LEVEL_H_DEFINED__
#include <algorithm>
#include <math.h>
#include <vector>
#include "Window.h"
#include "PugiXML.h"
#include "Sprite.h"

#define SIL_LAYER	0	// Silhouette layer
#define BG_LAYER	1	// Background layer
#define GAME_LAYER	2	// Game layer
#define FG_LAYER	3	// Foreground layer

#define LAYER_COUNT 4

#define SPAWN_LEFT	0
#define SPAWN_RIGHT 1

class Level
{

public:
	
	Level(Window *window);
	void load(std::string level_name);
	int getLevelWidth();
	int getLevelHeight();
	void render();
	pugi::xml_node Layer;



private:
	
	double getLineLength(int x1, int y1, int x2, int y2);
	double getLineLength(SDL_Point p1, SDL_Point p2);

	Window *window;
	
	pugi::xml_document levelDocument;
	pugi::xml_parse_result result;
	pugi::xml_node SilhouetteLayer, BackgroundLayer, GameLayer, ForegroundLayer;
	pugi::xml_node triggerNode, actorNode;


	int levelWidth, levelHeight;
	int tileSize;
	

	Sprite *levelTileSheet;
	std::vector<std::vector<int> > tileData;

	SDL_Point leftSpawn, rightSpawn, startSpawn;

};

#endif //__LEVEL_H_DEFINED__