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
	playername1(new Text(font, Color("white"))),
	playername2(new Text(font, Color("white"))),
	stateData(nullptr)
{
	timer.start();

}

void GameState::load(StateData *data)
{
	stateData = data;
	
	players = stateData->players;

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
					knights[i]->hit == false) {
					knights[j]->damage(15);
					knights[i]->hit = true;
				}
			}
		}
	}
	
	for (int i = 0; i < players; i++) {
		if (knights[i]->getHitpoints() <= 0) {
			knights[i]->alive = false;
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
	
	if (draw_healthbars) {
		window->drawRect(8,
						 12,
						 104,
						 10,
						 Color("black"));
		
		window->drawRect(10,
						 14,
						 knights[0]->getHitpoints(),
						 6,
						 Color("red"));

		window->drawRect(camera->getFrame().w - 104 - 8,
						 12,
						 104,
						 10,
						 Color("black"));
		
		window->drawRect(camera->getFrame().w - 104 - 6,
						 14,
						 knights[1]->getHitpoints(),
						 6,
						 Color("red"));

		playername1->print(window, knights[0]->getTruename(), 12, 2);
		playername2->print(window, knights[1]->getTruename(), camera->getFrame().w - 104 - 2, 2);
	}
}

StateData *GameState::getStateData()
{
	return nullptr;
}
