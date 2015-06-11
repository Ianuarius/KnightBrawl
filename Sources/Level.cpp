/**
 * Level.cpp
 *
 */

#include "Level.h"

Level::Level(Window *window, Camera *camera):
	window(window),
	camera(camera)
{
	start_position.x = 640;
	start_position.y = 480;
}

Level::~Level()
{
	delete levelTileSheet;
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

	int iteratorCount = 0;
	std::vector<int> levelRow;
	pugi::xml_node Layer = levelDocument.child("map").child("layer").child("data");

	for(pugi::xml_node_iterator iterator = Layer.begin();
				iterator != Layer.end();
				++iterator)
	{
		iteratorCount++;
		int gid = atoi(iterator->attribute("gid").value());
		levelRow.push_back(gid);
		
		if (iteratorCount % levelWidth == 0)
		{
			tileData.push_back(levelRow);
			levelRow.clear();
		}
	}

	//std::string tileSet = levelDocument.child("map").child("tileset").child("image").attribute("source").value();
	std::string tileSet = "testi.png";


	levelTileSheet = new Sprite(window, tileSet, tileSize, tileSize);
}

void Level::render()
{
	std::vector<std::vector<int>>::iterator row;
	std::vector<int>::iterator col;
	std::vector<std::vector<int>>::iterator row_begin;
	std::vector<std::vector<int>>::iterator row_end;
	std::vector<std::vector<int>> *data;
	data = &tileData;

	row_begin = data->begin();
	row_end = data->end();

	for (row = row_begin; row != row_end; ++row) {
		std::vector<int>::iterator col_begin = row->begin();
		std::vector<int>::iterator col_end = row->end();

		for (col = col_begin; col != col_end; ++col) {
			int X = col - row->begin();
			int Y = row - data->begin();
			
			int renderPosX = X*tileSize - camera->getFrame().x;
			int renderPosY = Y*tileSize - camera->getFrame().y;

			if ((*col) != 0) {
				levelTileSheet->setIndex(*col-1);
				levelTileSheet->render(renderPosX, renderPosY);
			}
			
		}
	}
}