/**
 * Level.h
 *
 */

#ifndef __LEVEL_H_DEFINED__
#define __LEVEL_H_DEFINED__

#include <algorithm>
#include <math.h>
#include <vector>
#include "Camera.h"
#include "PugiXML.h"
#include "Rectangle.h"
#include "Sprite.h"
#include "Window.h"

#define BG2_LAYER	0	// Background 2 layer
#define BG1_LAYER	1	// Background 1 layer
#define FG2_LAYER	2	// Foreground 2 layer
#define FG1_LAYER	3	// Foreground 1 layer
#define GAME_LAYER	4	// Game layer
#define PF_LAYER	5	// Platform layer

#define LAYER_COUNT 6

class Level
{
public:
	Level(Window *window, Camera *camera);
	~Level();

	void load(std::string level_name, std::string new_tileset);
	void render(int layer);
	int getTile(int x, int y);
	void collides(PlayerController *playerController);
	SDL_Point start_position;
	SDL_Rect pointToTile(int x, int y);

private:
	Window *window;
	Camera *camera;
	
	pugi::xml_document levelDocument;
	pugi::xml_parse_result result;

	pugi::xml_node Background2, Background1, 
				   Foreground2, Foreground1, 
				   GameLayer, PlatformLayer;

	std::vector<std::vector<int> > Background2Data, Background1Data, 
								   Foreground2Data, Foreground1Data, 
								   GameLayerData, PlatformLayerData;

	int levelWidth, levelHeight;
	int tileSize;
	
	Sprite *levelTileSheet;
};

#endif //__LEVEL_H_DEFINED__