/**
 * GameState.cpp
 *
 */

#include "GameState.h"

/**
 * TODO(juha):
 * [x] Pelaajien animaatioiden lukeminen XML:stä
 * [ ] Pelaajien liikkeiden yhdistäminen näppäimiin
 * [ ] Iskujen hitboxit ja niiden yhdistäminen tiettyyn animaation frameen
 * [ ] Combo-liikkeiden näppäinsarjat
 * 
 * 
 * 
 */

GameState::GameState(Window *window, Input *mainInput):
	window(window),
	mainInput(mainInput),
	camera(nullptr),
	knight1(nullptr),
	knight2(nullptr),
	level(nullptr),
	font(new Font("ChicagoFLF.ttf", 10)),
	playername1(new Text(font, Color("white"))),
	playername2(new Text(font, Color("white")))
{
	timer.start();
	SDL_Point start_point_1 = {36*16, 48*16};
	SDL_Point start_point_2 = {46*16, 48*16};

	bool multiplayer = true;

	if (PLAYERS <= 1) {
		multiplayer = false;
	}

	knight1 = new Knight(window, DRAGON_KNIGHT);
	knight2 = new Knight(window, CACTUS_KNIGHT);

	startPoints.push_back(start_point_1);
	startPoints.push_back(start_point_2);
	
	/*
	for (int i = 0; i < PLAYERS; i++) {
		playerControllers.push_back(new PlayerController(startPoints[i], multiplayer, i));
	}
	*/

	playerControllers.push_back(new PlayerController(startPoints[0], multiplayer, 0, knight1));
	playerControllers.push_back(new PlayerController(startPoints[1], multiplayer, 1, knight2));

	camera = new Camera(RESOLUTION_WIDTH, RESOLUTION_HEIGHT, &playerControllers);

	/*
	for (int i = 0; i < PLAYERS; i++) {
		playerActors.push_back(new PlayerActor(window, camera, playerControllers[i]));
	}
	*/
	
	playerActors.push_back(new PlayerActor(window, camera, playerControllers[0], knight1));
	playerActors.push_back(new PlayerActor(window, camera, playerControllers[1], knight2));
	
	level = new Level(window, camera);
	level->load("Levels/tavern_small.tmx");

	/*
	Pellaaaja lyöpi
	- tarkista painettiinko actionia
	- player_c:ssä attack = true
	- 

	*/
}

stateStatus GameState::update()
{
	stateStatus status;
	status.status = STATE_CONTINUE;
	status.prepend = false;
	
	if (mainInput->keyState(SDL_SCANCODE_ESCAPE)) {
		status.status = STATE_QUIT;
	}
	
	for (int i = 0; i < PLAYERS; i++) {
		playerControllers[i]->update();
	}
	
	for (int i = 0; i < PLAYERS; i++) {
		playerControllers[i]->updateInput();
	}

	for (int i = 0; i < PLAYERS; i++) {
		level->collides(playerControllers[i]);
	}
	
	for (int i = 0; i < PLAYERS; i++) {
		playerActors[i]->updateAnimation();
	}
	
	/*
		WEAPON HITBOX STUFF
	
			SDL_Rect tmp_hb = (*it)->hitbox;
			SDL_Rect wep_hb = player->weapon_hitbox;

			if (SDL_HasIntersection(&tmp_hb, &wep_hb)) {
				(*it)->damage(10);
			}

			if ((*it)->isDead()) {
				it = enemies.erase(it);
			}

			SDL_Rect plr_hb = player->hitbox;
			SDL_Rect enemy_wep_hb = (*it)->weapon_hitbox;

			if (SDL_HasIntersection(&plr_hb, &enemy_wep_hb)) {
				player->damage(1);
			}

	*/
	
	SDL_Rect tmp_hb = playerControllers[1]->hitbox;
	SDL_Rect wep_hb = playerControllers[0]->attack_hb;

	if (SDL_HasIntersection(&tmp_hb, &wep_hb) &&
		knight1->hit == false) {
		knight2->damage(15);
		knight1->hit = true;
	}
	
	tmp_hb = playerControllers[0]->hitbox;
	wep_hb = playerControllers[1]->attack_hb;

	if (SDL_HasIntersection(&tmp_hb, &wep_hb) &&
		knight2->hit == false) {
		knight1->damage(15);
		knight2->hit = true;
	}

	if (knight1->getHitpoints() <= 0) {
		knight1->alive = false;
	}
	
	if (knight2->getHitpoints() <= 0) {
		knight2->alive = false;
	}
	
	if (knight1->alive == true) {
		playerControllers[0]->commitMovement();
	}
	
	if (knight2->alive == true) {
		playerControllers[1]->commitMovement();
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

	for (int i = 0; i < PLAYERS; i++) {
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
						 knight1->getHitpoints(),
						 6,
						 Color("red"));

		window->drawRect(camera->getFrame().w - 104 - 8,
						 12,
						 104,
						 10,
						 Color("black"));
		
		window->drawRect(camera->getFrame().w - 104 - 6,
						 14,
						 knight2->getHitpoints(),
						 6,
						 Color("red"));

		playername1->print(window, knight1->getTruename(), 12, 2);
		playername2->print(window, knight2->getTruename(), camera->getFrame().w - 104 - 2, 2);

	}

}

