/**
 * GameState.cpp
 *
 */

#include "GameState.h"

GameState::GameState(Window *window, Input *mainInput):
	window(window),
	mainInput(mainInput),
	camera(nullptr),
	level(nullptr),
	font(new Font("ChicagoFLF.ttf", 10)),
	stateData(nullptr),
	coin(window, "Graphics/GUI/coin2.png")
{
	timer.start();

}

void GameState::load(StateData *data)
{
	stateData = data;
	
	players = stateData->players;

	for (int i = 0; i < players; i++) {
		playernames.push_back(new Text(font, Color("white")));
	}

	bool multiplayer = true;

	if (players <= 1) {
		multiplayer = false;
	}
	SDL_Point start_point_1 = {(stateData->start_x + 0) * 16, stateData->start_y * 16};
	SDL_Point start_point_2 = {(stateData->start_x + 6) * 16, stateData->start_y * 16};
	SDL_Point start_point_3 = {(stateData->start_x + 12) * 16, stateData->start_y * 16};
	SDL_Point start_point_4 = {(stateData->start_x + 18) * 16, stateData->start_y * 16};

	for (int i = 0; i < players; i++) {
		knights.push_back(stateData->selection[i]);
	}

	startPoints.push_back(start_point_1);
	startPoints.push_back(start_point_2);
	startPoints.push_back(start_point_3);
	startPoints.push_back(start_point_4);
	
	for (int i = 0; i < players; i++) {
		playerControllers.push_back(new PlayerController(startPoints[i], multiplayer, i, knights[i]));
	}
	
	camera = new Camera(RESOLUTION_WIDTH, RESOLUTION_HEIGHT, &playerControllers);

	for (int i = 0; i < players; i++) {
		playerActors.push_back(new PlayerActor(window, camera, playerControllers[i], knights[i]));
	}
	
	level = new Level(window, camera);
	level->load(stateData->level_path, stateData->level_tileset);
}

stateStatus GameState::update()
{
	stateStatus status;
	status.status = STATE_CONTINUE;
	status.prepend = false;
	
	int alive_knights = 0;

	for (int i = 0; i < players; i++) {
		if (knights[i]->getLives() > 0) {
			alive_knights++;
		}
	}

	if (alive_knights <= 1) {
		status.status = STATE_VICTORY;

		for (int i = 0; i < players; i++) {

		}
	}

	if (mainInput->keyState(SDL_SCANCODE_ESCAPE)) {
		status.status = STATE_QUIT;
	}
	
	for (int i = 0; i < players; i++) {
		playerControllers[i]->update();
	}
	
	for (int i = 0; i < players; i++) {
		playerControllers[i]->updateInput();
	}

	for (int i = 0; i < players; i++) {
		level->collides(playerControllers[i]);
	}
	
	for (int i = 0; i < players; i++) {
		playerActors[i]->updateAnimation();
		playerActors[i]->updateSound();
	}
	
	SDL_Rect tmp_hb;
	SDL_Rect wep_hb;
	
	for (int i = 0; i < players; i++) {
		wep_hb = playerControllers[i]->attack_hb;
		
		for (int j = 0; j < players; j++) {

			if (i != j) {
				tmp_hb = playerControllers[j]->hitbox;

				if (SDL_HasIntersection(&tmp_hb, &wep_hb) &&
					knights[i]->hit == false &&
					knights[j]->alive == true) {
					knights[j]->damage(50);
					knights[i]->hit = true;
					knights[i]->powerup();

					if (knights[j]->getHitpoints() <= 0 &&
						knights[j]->alive == true) {
						stateData->player_kills[i]++;
					}
				}
			}
		}
	}
	
	for (int i = 0; i < players; i++) {
		if (knights[i]->getHitpoints() <= 0) {
			knights[i]->kill();
		}
	}
	
	for (int i = 0; i < players; i++) {
		if (playerControllers[i]->boundbox.x < 0 ||
			playerControllers[i]->boundbox.x > level->getWidth() ||
			playerControllers[i]->boundbox.y > level->getHeight()) {
			knights[i]->kill();
			playerControllers[i]->desired.x = startPoints[i].x;
			playerControllers[i]->desired.y = startPoints[i].y;
			playerControllers[i]->commitMovement();
		}
	}
	
	for (int i = 0; i < players; i++) {
		if (knights[i]->alive == true) {
			playerControllers[i]->commitMovement();
		}
	}
	
	
	mainInput->update();

	camera->update();

	// NOTE(juha): prints player location to console
	/*
	printf("Player x: %d - Player y: %d\n", 
		playerControllers[0]->location.x, playerControllers[0]->location.y);
	*/
	return status;
}

void GameState::render()
{
	level->render(BG2_LAYER);
	level->render(BG1_LAYER);
	level->render(GAME_LAYER);
	
	level->render(PF_LAYER);
	level->render(FG2_LAYER);
	level->render(FG1_LAYER);

	for (int i = 0; i < players; i++) {
		playerActors[i]->render();
	}
	
	bool draw_healthbars = true;
	int middle_region = (camera->getFrame().w - ((104 + 8) * 2));
	int middle_padding = (middle_region - (104 + 8) * 2) / 3;

	std::vector<int> left_margins;

	switch (players)
	{
	case 1:
		left_margins.push_back(8);

	case 2:
		left_margins.push_back(8);
		left_margins.push_back(camera->getFrame().w - 104 - 8);

	case 3:
		left_margins.push_back(8);
		left_margins.push_back(camera->getFrame().w / 2 - 104 / 2);
		left_margins.push_back(camera->getFrame().w - 104 - 8);
		
	case 4:
		left_margins.push_back(8);
		left_margins.push_back((104 + 8) + middle_padding);
		left_margins.push_back(((104 + 8) + middle_padding) * 2);
		left_margins.push_back(camera->getFrame().w - 104 - 8);

	default:
		break;
	}

	int top_margin = 12;
	
	if (draw_healthbars) {

		for (int i = 0; i < players; i++) {
			window->drawRect(left_margins[i],
							 top_margin,
							 104,
							 10,
							 Color("black"));
		
			window->drawRect(left_margins[i] + 2,
							 top_margin + 2,
							 knights[i]->getHitpoints(),
							 6,
							 Color("red"));
			
			window->drawRect(left_margins[i],
							 top_margin + 8,
							 104,
							 6,
							 Color("black"));
		
			window->drawRect(left_margins[i] + 2,
							 top_margin + 10,
							 knights[i]->getSpecialPower(),
							 2,
							 Color(0xAABB77));

			// top margin 12, 10, 6

			for (int j = 0; j < knights[i]->getLives(); j++) {
				coin.render(left_margins[i] + 2 + (j * 9),
							top_margin + 16);
			}

			playernames[i]->print(window, knights[i]->getTruename(), left_margins[i] + 4, top_margin - 12);
		}
	}
}

StateData *GameState::getStateData()
{	
	for (int i = 0; i < players; i++) {
		stateData->player_deaths[i] = knights[i]->getDeaths();
	}
	
	return stateData;
}
