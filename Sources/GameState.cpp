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
	ost("../Music/knight_brawl.ogg"),
	font(new Font("../Fonts/ChicagoFLF.ttf", 10)),
	stateData(nullptr),
	victory_counter(0),
	coin(window, "../Graphics/GUI/coin2.png")
{
	timer.start();

}

GameState::~GameState()
{
}

void GameState::load(StateData *data)
{
	stateData = data;
	bool music = false;
	
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
	
	// NOTE(juha): If there are gamepads connected to the PC,
	// assign them to the 4th and 3rd player, respectively.
	if (stateData->ControllerIndex == 2 && stateData->players == 4) {
		playerControllers[2]->setGamepad(stateData->ControllerHandles[0]);
		playerControllers[3]->setGamepad(stateData->ControllerHandles[1]);
	} else if (stateData->ControllerIndex == 1 && stateData->players == 3) {
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

	if (music) {
		ost.play();
	}
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
			ost.stop();
			status.status = STATE_VICTORY;
		}

		for (int i = 0; i < players; i++) {

		}
	}

	if (mainInput->keyState(SDL_SCANCODE_ESCAPE)) {
		ost.stop();
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

	SDL_Rect attacking_hb;
	
	// NOTE(juha): Check basic hits
	for (int attacking_knight = 0; 
		attacking_knight < players; 
		attacking_knight++) {
		attacking_hb = playerControllers[attacking_knight]->attack_hb;

		checkHits(&attacking_hb, attacking_knight, 
			playerControllers[attacking_knight]->getKnight()->getMoves()->at(3).damage,
			playerControllers[attacking_knight]->getDirection());
	}
	
	// NOTE(juha): Check projectile hits
	for (int a_projectile = 0; 
		a_projectile < projectiles.size(); 
		a_projectile++) {
		attacking_hb = projectiles[a_projectile].hitbox;
		
		checkHits(&attacking_hb, projectiles[a_projectile].player,
			projectiles[a_projectile].power,
			projectiles[a_projectile].direction);
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

void GameState::checkHits(SDL_Rect *attacking_hb, int attacker, int damage, int direction)
{
	SDL_Rect receiving_hb;
	int damage_multiplier = 2;

	for (int receiver = 0; 
		receiver < players; 
		receiver++) {

		if (attacker != receiver) {
			receiving_hb = playerControllers[receiver]->hitbox;

			if (SDL_HasIntersection(&receiving_hb, attacking_hb) &&
				knights[attacker]->hit == false &&
				knights[receiver]->alive == true) {
				knights[receiver]->damage(damage * damage_multiplier);
				knights[attacker]->hit = true;
				knights[attacker]->powerup();

				if (knights[receiver]->getHitpoints() <= 0 &&
					knights[receiver]->alive == true) {
					stateData->player_kills[attacker]++;
					knights[receiver]->die();
				} else {
					playerControllers[receiver]->knockBack(direction,
														   playerControllers[attacker]->pushback_angle,
														   playerControllers[attacker]->pushback_power * 3);
				}
			}
		}
	}
}

void GameState::executeMoves(int knight, int move)
{
	if (knights[knight]->getMoves()->at(move).executing) {

		if (knights[knight]->getMoves()->at(move).start_execution == false) {

			knights[knight]->getMoves()->at(move).start_execution = true;

			// NOTE(juha): go through the projectile spawners
			if (knights[knight]->getMoves()->at(move).projectile_spawners.size() > 0) {
				
				ProjectileSpawner tmp_pspawner = knights[knight]->
					getMoves()->at(move).projectile_spawners[0];

				for (int i = 0; i < tmp_pspawner.amount; i++) {
					Projectile tmp_projectile = knights[knight]->getProjectiles()->at(0);
					tmp_projectile.direction = playerControllers[knight]->getDirection();
					tmp_projectile.x_coordinate = playerControllers[knight]->
						location.x + tmp_projectile.x_offset;
					tmp_projectile.y_coordinate = playerControllers[knight]->
						location.y + tmp_projectile.y_offset;
					tmp_projectile.angle = tmp_pspawner.angle + (tmp_pspawner.angle_interval * i);
					tmp_projectile.player = knight;
					tmp_projectile.animation->play(INFINITE_LOOP);
					projectiles.push_back(tmp_projectile);
				}
			}

			// NOTE(juha): go through the effects
			if (knights[knight]->getMoves()->at(move).effects.size() > 0) {
				for (int i = 0; i < knights[knight]->getMoves()->at(move).effects.size(); i++) {
					
					Effect *tmp_effect = &knights[knight]->getMoves()->at(move).effects[i];
					tmp_effect->executing = true;

					switch (tmp_effect->type) {
					case 0: // slow
						break;
					case 1: // stun
						break;
					case 2: // burn
						break;
					case 3: // pushback
						playerControllers[knight]->pushback_angle = tmp_effect->angle;
						playerControllers[knight]->pushback_power = tmp_effect->power;
						break;
					case 4: // buff
						break;
					case 5: // damage_return
						break;
					case 6: // movement
						playerControllers[knight]->movements.push_back(tmp_effect);
						break;
					}
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
