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
	font(new Font("../Fonts/ChicagoFLF.ttf", 10)),
	stateData(nullptr),
	victory_counter(0),
	coin(window, "../Graphics/GUI/coin2.png")
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
	
	if (stateData->ControllerIndex == 2) {
		playerControllers[2]->setGamepad(stateData->ControllerHandles[0]);
		playerControllers[3]->setGamepad(stateData->ControllerHandles[1]);
	} else if (stateData->ControllerIndex == 1) {
		playerControllers[3]->setGamepad(stateData->ControllerHandles[0]);
	} else if (stateData->ControllerIndex == 0) {
		// nobody gamepads rip
	}

	camera = new Camera(RESOLUTION_WIDTH, RESOLUTION_HEIGHT, &playerControllers, players);

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
		victory_counter++;

		if (victory_counter > 120) {
			status.status = STATE_VICTORY;
		}

		for (int i = 0; i < players; i++) {

		}
	}

	if (mainInput->keyState(SDL_SCANCODE_ESCAPE)) {
		status.status = STATE_MENU;
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
		if (playerControllers[i]->boundbox.x < 0 ||
			playerControllers[i]->boundbox.x > level->getWidth() ||
			playerControllers[i]->boundbox.y > level->getHeight()) {
			knights[i]->out_of_bounds = true;
			knights[i]->die();
			playerControllers[i]->desired.x = startPoints[i].x;
			playerControllers[i]->desired.y = startPoints[i].y;
			playerControllers[i]->commitMovement();
		}
	}
	
	for (int i = 0; i < players; i++) {
		playerActors[i]->updateAnimation();
		playerActors[i]->updateSound();
	}
	
	for (int i = 0; i < players; i++) {
		for (int j = 0; j < knights[i]->getMoves()->size(); j++) {
			executeMoves(i, j);
		}
	}

	for (int i = 0; i < projectiles.size(); i++) {
		projectiles[i].update();
		level->collides(&projectiles[i]);
	}
	
	for (int i = 0; i < projectiles.size(); i++) {
		
		if (projectiles[i].collision == true) {
			projectiles.erase(projectiles.begin() + i);
		}
	}

	SDL_Rect tmp_hb;
	SDL_Rect wep_hb;
	
	for (int attacking_knight = 0; 
		attacking_knight < players; 
		attacking_knight++) {
		wep_hb = playerControllers[attacking_knight]->attack_hb;
		
		for (int receiving_knight = 0; 
			receiving_knight < players; 
			receiving_knight++) {

			if (attacking_knight != receiving_knight) {
				tmp_hb = playerControllers[receiving_knight]->hitbox;

				if (SDL_HasIntersection(&tmp_hb, &wep_hb) &&
					knights[attacking_knight]->hit == false &&
					knights[receiving_knight]->alive == true) {
					knights[receiving_knight]->damage(5);
					knights[attacking_knight]->hit = true;
					knights[attacking_knight]->powerup();

					if (knights[receiving_knight]->getHitpoints() <= 0 &&
						knights[receiving_knight]->alive == true) {
						stateData->player_kills[attacking_knight]++;
						knights[receiving_knight]->die();
					}
				}
			}
		}
	}
	
	for (int a_projectile = 0; 
		a_projectile < projectiles.size(); 
		a_projectile++) {
		wep_hb = projectiles[a_projectile].hitbox;
		
		for (int receiving_knight = 0; 
			receiving_knight < players; 
			receiving_knight++) {

			if (projectiles[a_projectile].player != receiving_knight) {
				tmp_hb = playerControllers[receiving_knight]->hitbox;

				if (SDL_HasIntersection(&tmp_hb, &wep_hb) &&
					projectiles[a_projectile].hit == false &&
					knights[receiving_knight]->alive == true) {
					knights[receiving_knight]->damage(50);
					projectiles[a_projectile].hit = true;

					if (knights[receiving_knight]->getHitpoints() <= 0 &&
						knights[receiving_knight]->alive == true) {
						stateData->player_kills[projectiles[a_projectile].player]++;
						knights[receiving_knight]->die();
					}
				}
			}
		}
	}
	
	for (int i = 0; i < players; i++) {
		if (knights[i]->alive == true) {
			playerControllers[i]->commitMovement();
		}
	}
	
	mainInput->update();
	camera->update();
	return status;
}

void GameState::executeMoves(int knight, int move)
{
	if (knights[knight]->getMoves()->at(move).executing) {

		if (knights[knight]->getMoves()->at(move).start_execution == false) {

			knights[knight]->getMoves()->at(move).start_execution = true;

			// NOTE(juha): go through the projectile spawners
			if (knights[knight]->getMoves()->at(move).projectile_spawners.size() > 0) {
				
				ProjectileSpawner tmp_pspawner = knights[knight]->getMoves()->at(move).projectile_spawners[0];

				for (int i = 0; i < tmp_pspawner.amount; i++) {
					Projectile tmp_projectile = knights[knight]->getProjectiles()->at(0);
					tmp_projectile.direction = playerControllers[knight]->getDirection();
					tmp_projectile.x_coordinate = playerControllers[knight]->location.x + tmp_projectile.x_offset;
					tmp_projectile.y_coordinate = playerControllers[knight]->location.y + tmp_projectile.y_offset;
					tmp_projectile.angle = tmp_pspawner.angle + (tmp_pspawner.angle_interval * i);
					tmp_projectile.player = knight;
					tmp_projectile.animation->play(INFINITE_LOOP);
					projectiles.push_back(tmp_projectile);
				}
			}
		}
	}

	if (knights[knight]->getMoves()->at(move).executing == false &&
		knights[knight]->getMoves()->at(move).start_execution == true) {
		knights[knight]->getMoves()->at(move).start_execution = false;
	}
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
	
	if (projectiles.size() > 0) {
		for (int i = 0; i < projectiles.size(); i++) {

			if (projectiles[i].direction == 2) {
				projectiles[i].animation->flip = true;
			} else  {
				projectiles[i].animation->flip = false;
			}

			projectiles[i].animation->render(projectiles[i].location.x - camera->getFrame().x,
											 projectiles[i].location.y - camera->getFrame().y);
		}
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
