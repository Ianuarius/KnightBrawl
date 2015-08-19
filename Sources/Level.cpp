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

void Level::load(std::string level_name, std::string new_tileset)
{					 
	result = levelDocument.load_file(level_name.c_str());

	if (!result) {
		printf("Failed to load level %s.\n", level_name.c_str());
		return;
	}
		
	tileSize = atoi(levelDocument.child("map").attribute("tilewidth").value());
	levelWidth = atoi(levelDocument.child("map").attribute("width").value());
	levelHeight = atoi(levelDocument.child("map").attribute("height").value());
	
	// Get layer and object nodes
	Background2		= levelDocument.child("map").find_child_by_attribute("name", "Background2").child("data");
	Background1		= levelDocument.child("map").find_child_by_attribute("name", "Background1").child("data");
	Foreground2		= levelDocument.child("map").find_child_by_attribute("name", "Foreground2").child("data");
	Foreground1		= levelDocument.child("map").find_child_by_attribute("name", "Foreground1").child("data");
	GameLayer		= levelDocument.child("map").find_child_by_attribute("name", "GameLayer").child("data");
	PlatformLayer	= levelDocument.child("map").find_child_by_attribute("name", "PlatformLayer").child("data");
	
	
	levelTileSheet = new Sprite(window, new_tileset, tileSize, tileSize);

	int iteratorCount = 0;
	std::vector<int> levelRow;
	
	// Parse layers
	for(int i = 0; i < LAYER_COUNT; i++) {
		pugi::xml_node *tempLayer = nullptr;
		std::vector<std::vector<int>> *tempData = nullptr;

		switch (i)
		{
			case BG2_LAYER:
				tempLayer = &Background2;
				tempData = &Background2Data;
				break;

			case BG1_LAYER:
				tempLayer = &Background1;
				tempData = &Background1Data;
				break;
				
			case FG2_LAYER:
				tempLayer = &Foreground2;
				tempData = &Foreground2Data;
				break;

			case FG1_LAYER:
				tempLayer = &Foreground1;
				tempData = &Foreground1Data;
				break;

			case GAME_LAYER:
				tempLayer = &GameLayer;
				tempData = &GameLayerData;
				break;

			case PF_LAYER:
				tempLayer = &PlatformLayer;
				tempData = &PlatformLayerData;
				break;

			default:
				break;
		}
		
		for(pugi::xml_node_iterator iterator = tempLayer->begin();
			iterator != tempLayer->end();
			++iterator)
		{
			iteratorCount++;
			
			int gid = atoi(iterator->attribute("gid").value());
			levelRow.push_back(gid);

			if (iteratorCount % levelWidth == 0)
			{
				tempData->push_back(levelRow);
				levelRow.clear();
			}
		}
		
	}
}

int Level::getTile(int x, int y)
{
	if (y >= 0 &&
		x >= 0 &&
		y < (int)GameLayerData.size()*tileSize &&
		x < (int)GameLayerData[0].size()*tileSize)
	{
		return (GameLayerData[y/tileSize][x/tileSize]);
	}

	return 0;
}

SDL_Rect Level::pointToTile(int x, int y) {
	int correction_x = x % tileSize;
	int correction_y = y % tileSize;

	int tile_x = x - correction_x;
	int tile_y = y - correction_y;

	SDL_Rect result = {tile_x, tile_y, tileSize, tileSize};

	return result;
}

void Level::collides(PlayerController *playerController)
{
	// Top collision
    if (getTile(playerController->desired.Center().x, playerController->desired.y) != 0) {
        SDL_Rect top_collision_tile = pointToTile(playerController->desired.Center().x, playerController->desired.y);

        playerController->desired.y = top_collision_tile.y + top_collision_tile.h;
        playerController->velocity_y = 0;
    }

    // Horizontal collision

	// Get the area of tiles considered for vertical collision
    // @see http://i.imgur.com/hfOKWO3.png
	SDL_Point min_tile_pos = {std::min(playerController->hitbox.TopLeft().x, playerController->desired.TopLeft().x) / tileSize,
							  std::min(playerController->hitbox.TopLeft().y, playerController->desired.TopLeft().y) / tileSize};

	SDL_Point max_tile_pos = {std::max(playerController->hitbox.BottomRight().x, playerController->desired.BottomRight().x) / tileSize,
                             (std::max(playerController->hitbox.BottomRight().y, playerController->desired.BottomRight().y) + 10) / tileSize};

    // Cast entity->desired to SDL_Rect since SDL_HasIntersection requires it
    SDL_Rect sdl_desired = (SDL_Rect) playerController->desired;

    // Loop through all tiles considered for collision
	for (int y_tile = min_tile_pos.y; y_tile < max_tile_pos.y; y_tile++) {
		for (int x_tile = min_tile_pos.x; x_tile <= max_tile_pos.x; x_tile++) {

			SDL_Rect tile = {(x_tile * tileSize), (y_tile * tileSize),
							tileSize, tileSize};
			
            // Correct desired position if tile is collidable and it has intersection
            // width desired
			if (SDL_HasIntersection(&tile, &sdl_desired) &&
				getTile(x_tile * tileSize, y_tile * tileSize) != 0) {

				if (tile.x > playerController->desired.x) {
                    // Correct right
					playerController->desired.x = tile.x - playerController->hitbox.w;
				} else {
                    // Correct left
					playerController->desired.x = tile.x + tile.w;
				}
			} 
		}
	}

	// Bottom collision
    SDL_Rect bot_collision_tile = {0, 0, 0, 0};

    // TODO(jouni): Translate plz
    //
    // Väsyttää. -_-
    // Bottom collisionissa riittää että tarkiestetaan desiredin keskeltä, 2/4 kokoiselta alueelta (desiredin
    // leveydestä), jos siellä on collidable tile niin ei tarvitte tippua.
    // Hyvänä tai huonona puolena; jos on liian reunalla tileä (1/4 desiredin koosta), entiteetti luiskahtaa
    // tileltä alas. RIP.

    if (getTile(playerController->desired.BottomLeft().x + (playerController->desired.w / 16), playerController->desired.BottomLeft().y) != 0) {
        bot_collision_tile = pointToTile(playerController->desired.BottomLeft().x + (playerController->desired.w / 16), playerController->desired.BottomLeft().y);
    }

    if (getTile(playerController->desired.BottomRight().x - (playerController->desired.w / 16), playerController->desired.BottomRight().y) != 0) {
        bot_collision_tile = pointToTile(playerController->desired.BottomRight().x - (playerController->desired.w / 16), playerController->desired.BottomRight().y);
    }

    if (!SDL_RectEmpty(&bot_collision_tile)) {
        playerController->desired.y = bot_collision_tile.y - playerController->hitbox.h;

        if (playerController->velocity_y >= 0) {
            playerController->velocity_y = 0;
            playerController->in_air = false;
			
			playerController->getKnight()->is_landed = true;
        }
    }

	if (camera->movementLocked()) {
		playerController->desired.x = playerController->hitbox.x;
	}
}

void Level::collides(Projectile *projectile)
{
	// Top collision
    if (getTile(projectile->hitbox.Center().x, projectile->hitbox.y) != 0) {
        SDL_Rect top_collision_tile = pointToTile(projectile->hitbox.Center().x, projectile->hitbox.y);

        projectile->collision = true;
    }

    // Horizontal collision=
	SDL_Point min_tile_pos = {std::min(projectile->hitbox.TopLeft().x, projectile->hitbox.TopLeft().x) / tileSize,
							  std::min(projectile->hitbox.TopLeft().y, projectile->hitbox.TopLeft().y) / tileSize};

	SDL_Point max_tile_pos = {std::max(projectile->hitbox.BottomRight().x, projectile->hitbox.BottomRight().x) / tileSize,
                              std::max(projectile->hitbox.BottomRight().y, projectile->hitbox.BottomRight().y) / tileSize};

    // Cast entity->hitbox to SDL_Rect since SDL_HasIntersection requires it
    SDL_Rect sdl_hitbox = (SDL_Rect) projectile->hitbox;

    // Loop through all tiles considered for collision
	for (int y_tile = min_tile_pos.y; y_tile < max_tile_pos.y; y_tile++) {
		for (int x_tile = min_tile_pos.x; x_tile <= max_tile_pos.x; x_tile++) {

			SDL_Rect tile = {(x_tile * tileSize), (y_tile * tileSize),
							tileSize, tileSize};
			
            // Correct hitbox position if tile is collidable and it has intersection
            // width hitbox
			if (SDL_HasIntersection(&tile, &sdl_hitbox) &&
				getTile(x_tile * tileSize, y_tile * tileSize) != 0) {
					
				projectile->collision = true;
			}
		}
	}

	// Bottom collision
    SDL_Rect bot_collision_tile = {0, 0, 0, 0};

    if (getTile(projectile->hitbox.BottomLeft().x + (projectile->hitbox.w / 16), projectile->hitbox.BottomLeft().y) != 0) {
        bot_collision_tile = pointToTile(projectile->hitbox.BottomLeft().x + (projectile->hitbox.w / 16), projectile->hitbox.BottomLeft().y);
    }

    if (getTile(projectile->hitbox.BottomRight().x - (projectile->hitbox.w / 16), projectile->hitbox.BottomRight().y) != 0) {
        bot_collision_tile = pointToTile(projectile->hitbox.BottomRight().x - (projectile->hitbox.w / 16), projectile->hitbox.BottomRight().y);
    }

    if (!SDL_RectEmpty(&bot_collision_tile)) {
        projectile->collision = true;
    }

}

void Level::render(int layer)
{
	std::vector<std::vector<int>>::iterator row;
	std::vector<int>::iterator col;
	std::vector<std::vector<int>>::iterator row_begin;
	std::vector<std::vector<int>>::iterator row_end;
	std::vector<std::vector<int>> *data = nullptr;
	
	switch (layer)
	{
		case BG2_LAYER:
			data = &Background2Data;
			break;

		case BG1_LAYER:
			data = &Background1Data;
			break;
				
		case FG2_LAYER:
			data = &Foreground2Data;
			break;

		case FG1_LAYER:
			data = &Foreground1Data;
			break;

		case GAME_LAYER:
			data = &GameLayerData;
			break;

		case PF_LAYER:
			data = &PlatformLayerData;
			break;

		default:
			break;
	}
	
	int tiles_y = camera->getFrame().h / tileSize + 2;
	int start_tile_y = (camera->getFrame().y / tileSize);
	if (start_tile_y < 0) {
		start_tile_y = 0;
	}

	int tiles_x = (camera->getFrame().w / tileSize) + 2;
	int start_tile_x = (camera->getFrame().x / tileSize);
	if (start_tile_x < 0) {
		start_tile_x = 0;
	}

	row_begin = data->begin() + start_tile_y;
	row_end = data->end();
	
	if (start_tile_y + tiles_y < (int)data->size()) {
		row_end = data->begin() + start_tile_y + tiles_y;
	} else {
		row_end = data->end();
	}
	
	for (row = row_begin; row != row_end; ++row) {
		std::vector<int>::iterator col_begin = row->begin() + start_tile_x;
		std::vector<int>::iterator col_end = row->end();
		if (start_tile_x + tiles_x < (int)row->size()) {
			col_end = row->begin() + start_tile_x + tiles_x;
		}

		for (col = col_begin; col != col_end; ++col) {
			int X = col - row->begin();
			int Y = row - data->begin();
			if ((*col) != 0) {
				levelTileSheet->setIndex(*col-1);
				int frameX = (camera->getFrame().x % 2 == 0) ? camera->getFrame().x : camera->getFrame().x + 1;
				levelTileSheet->render(X*tileSize - frameX, Y*tileSize - camera->getFrame().y);
			}
		}
	}

	/*
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
	*/
}

int Level::getWidth()
{
	return levelWidth * tileSize;
}

int Level::getHeight()
{
	return levelHeight * tileSize;
}