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
			TileData.push_back(levelRow);
			levelRow.clear();
		}
	}

	//std::string tileSet = levelDocument.child("map").child("tileset").child("image").attribute("source").value();
	std::string tileSet = "Graphics/Tilesets/tavern_tileset.png";


	levelTileSheet = new Sprite(window, tileSet, tileSize, tileSize);
}

int Level::getTile(int x, int y)
{
	if (y >= 0 &&
		x >= 0 &&
		y < TileData.size()*tileSize &&
		x < TileData[0].size()*tileSize)
	{
		return (TileData[y/tileSize][x/tileSize]);
	}

	return 0;
}

void Level::collides(PlayerController *playerController)
{
	Rectangle old_entity = playerController->hitbox;
	Rectangle new_entity = playerController->boundbox;
	new_entity.w = old_entity.w;
	new_entity.h = old_entity.h;

	// Get the area of tiles considered for collision
	SDL_Point min_tile_xy = {std::min(old_entity.TopLeft().x, new_entity.TopLeft().x),
							 std::min(old_entity.TopLeft().y, new_entity.TopLeft().y)};
	SDL_Point max_tile_xy = {std::max(old_entity.BottomRight().x, new_entity.BottomRight().x),
							 std::max(old_entity.BottomRight().y, new_entity.BottomRight().y)};

	int min_tile_x = (min_tile_xy.x / tileSize) - 1;
	int min_tile_y = (min_tile_xy.y / tileSize) - 1;
	int max_tile_x = (max_tile_xy.x / tileSize) + 1;
	int max_tile_y = (max_tile_xy.y / tileSize) + 1;

	SDL_Rect tmpbound = (SDL_Rect) playerController->hitbox;

	// Horizontal collision

	for (int y_tile = min_tile_y + 1; y_tile < max_tile_y - 1; y_tile++) {
		for (int x_tile = min_tile_x; x_tile <= max_tile_x; x_tile++) {

			SDL_Rect tmp_tile = {(x_tile * tileSize),
								(y_tile * tileSize),
								tileSize,
								tileSize-2};
			
			if (SDL_HasIntersection(&tmp_tile, &tmpbound) &&
				getTile(x_tile * tileSize, y_tile * tileSize) != 0) {

				// NOTE(juha): If player mid is more right than tile mid
				if ((playerController->location.x) > (tmp_tile.x + (tileSize / 2))) {
 					playerController->location.x = tmp_tile.x + tileSize + playerController->hitbox.w / 2;
				}
				
				// NOTE(juha): If player mid is more left than tile mid
				if ((playerController->location.x) < (tmp_tile.x + (tileSize / 2))) {
 					playerController->location.x = tmp_tile.x - playerController->hitbox.w / 2;
				}
			}
			/*
			SDL_Rect tmp_tile = {(x_tile * tileSize),
							(y_tile * tileSize),
							tileSize,
							tileSize-2};

			if (getTile(x_tile * tileSize, y_tile * tileSize) != 0) {
				if (getTile(x_tile * tileSize, (y_tile + 1) * tileSize) != 0) {
					tmp_tile.h += tileSize;
				}
			}

			if (SDL_HasIntersection(&tmp_tile, &tmpbound) &&
				getTile(x_tile * tileSize, y_tile * tileSize) != 0) {
					if (playerController->velocity_x > 0) {
						playerController->boundbox.x = tmp_tile.x - (playerController->hitbox.w + 1);
					} else {
						playerController->boundbox.x = (tmp_tile.x + tmp_tile.w) + 1;
					}
					
			}*/
		}
	}

	tmpbound = (SDL_Rect) playerController->hitbox;

	// Vertical collision
	for (int y_tile = min_tile_y; y_tile <= max_tile_y; y_tile++) {
		for (int x_tile = min_tile_x; x_tile < max_tile_x; x_tile++) {
			
			SDL_Rect tmp_tile = {(x_tile * tileSize),
							(y_tile * tileSize),
							tileSize,
							tileSize};

			if (getTile(x_tile * tileSize, y_tile * tileSize) != 0) {
				if (getTile((x_tile + 1) * tileSize, y_tile * tileSize) != 0) {
					tmp_tile.w += tileSize;
				}
			}



			if (SDL_HasIntersection(&tmp_tile, &tmpbound) &&
				getTile(x_tile * tileSize, y_tile * tileSize) != 0) {

				// NOTE(juha): If player mid is lower than tile mid
				if ((playerController->location.y - (playerController->hitbox.h / 2)) > (tmp_tile.y + (tileSize / 2))) {
 					playerController->location.y = tmp_tile.y + tileSize + playerController->hitbox.h;
				}
			
				// NOTE(juha): If player mid is higher than tile mid
				if ((playerController->location.y - (playerController->hitbox.h / 2)) < (tmp_tile.y + (tileSize / 2))) {
					playerController->location.y = tmp_tile.y;
					playerController->velocity_y = 0;
					playerController->in_air = false;
				}
					/*
					if (tmp_tile.y < playerController->boundbox.y) {
						playerController->boundbox.y = tmp_tile.y + tmp_tile.h + 1;
						playerController->velocity_y = 0;
					} else {
						playerController->boundbox.y = tmp_tile.y - playerController->hitbox.h;
						playerController->velocity_y = 0;
						playerController->in_air = false;
					}
					*/
			}
			
		}
	}
}

void Level::render()
{
	std::vector<std::vector<int>>::iterator row;
	std::vector<int>::iterator col;
	std::vector<std::vector<int>>::iterator row_begin;
	std::vector<std::vector<int>>::iterator row_end;
	std::vector<std::vector<int>> *data;
	data = &TileData;

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