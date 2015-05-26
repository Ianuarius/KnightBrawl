/**
 * Level.cpp
 *
 */

#include "Level.h"

Level::Level(Window *window):
	window(window)
{


}

void Level::load(std::string level_name)
{					 
	result = levelDocument.load_file(level_name.c_str());

	if (!result) {
		printf("Failed to load level %s.\n", level_name.c_str());
		return;
	}


	tileSize = atoi(levelDocument.child("map").attribute("tilewidth").value());
	levelWidth = atoi(levelDocument.child("map").attribute("width").value());
	levelHeight = atoi(levelDocument.child("map").attribute("height").value());

	std::string tileSet = levelDocument.child("map").child("tileset").child("image").attribute("source").value();

}

void Level::render(int layer)
{
	
	for (int i = 0; i < levelHeight; i++)
	{
		

		for (int j = 0; j < levelWidth; j++)
		{

		}
	}

}
