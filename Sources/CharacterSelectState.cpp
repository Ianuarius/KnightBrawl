/**
 * CharacterSelectState.cpp
 *
 */

#include "CharacterSelectState.h"

CharacterSelectState::CharacterSelectState(Window *window, Input *mainInput):
	window(window),
	mainInput(mainInput),
	font(new Font("../Fonts/ChicagoFLF.ttf", 16)),
	playerfont(new Font("../Fonts/ChicagoFLF.ttf", 12)),
	player_tag(new Font("../Fonts/ChicagoFLF.ttf", 8)),
	header(new Text(font, Color("white")))
{
	stateData = new StateData();

	// NOTE(juha): Initialization:
	bool multiplayer =	true;		// multiple players on same machine
	character_select =	true;		// are we in character select?
	level_select =		false;		// are we in level select?
	active_players =	2;			// how many of the max players are active
	max_players =		4;			// max amount of possible players
	lives_total =		3;			// the amount of lives in the match

	stateData->selection.resize(active_players);
	
	sfx_select1.load("../Sounds/select1.wav");

	// NOTE(juha): Initializing the menu texts.
	for (int i = 0; i < max_players; i++) {
		tags.push_back(new Text(player_tag, Color("white")));
		tag_shadows.push_back(new Text(player_tag, Color("black")));
		player_identifiers.push_back(new Text(playerfont, Color(0x4B692F)));
		player_names.push_back(new Text(playerfont, Color("white")));
	}
	
	tag_texts.push_back("P1");
	tag_texts.push_back("P2");
	tag_texts.push_back("P3");
	tag_texts.push_back("P4");
	
	id_texts.push_back("Player 1");
	id_texts.push_back("Player 2");
	id_texts.push_back("Player 3");
	id_texts.push_back("Player 4");
	
	// NOTE(juha): Initializing the text positions.
	SDL_Point tmp_point;

	tmp_point.x = 80;
	tmp_point.y = 100;
	id_positions.push_back(tmp_point);

	tmp_point.x = 420;
	tmp_point.y = 100;
	id_positions.push_back(tmp_point);

	tmp_point.x = 80;
	tmp_point.y = 150;
	id_positions.push_back(tmp_point);

	tmp_point.x = 420;
	tmp_point.y = 150;
	id_positions.push_back(tmp_point);
	
	// NOTE(juha): Initializing possible gamepads.
	SDL_InitSubSystem(SDL_INIT_GAMECONTROLLER);

	int MaxJoysticks = SDL_NumJoysticks();
	controller_index = 0;

	for (int joystick_index = 0; 
			 joystick_index < MaxJoysticks; 
		   ++joystick_index) {
		if (!SDL_IsGameController(joystick_index)) {
			continue;
		}
		if (controller_index >= MAX_CONTROLLERS) {
			break;
		}
		ControllerHandles[controller_index] = 
			SDL_GameControllerOpen(joystick_index);
		controller_index++;
	}
	
	// NOTE(juha): Go through the roster.xml and parse the data.
	roster_result = roster_doc.load_file("../Scripts/roster.xml");

	roster_row_length = 5;
	roster_row_count = 0;
	int roster_col_count = 0;
	std::vector<Knight *> roster_tmp_row;


	for(pugi::xml_node_iterator iterator = roster_doc.child("roster").begin();
								iterator != roster_doc.child("roster").end();
							  ++iterator) {
		roster_col_count++;
		roster_tmp_row.push_back(new Knight(window, 
			atoi(iterator->attribute("id").value()), lives_total));

		if (roster_col_count >= roster_row_length) {
			knights.push_back(roster_tmp_row);
			roster_tmp_row.clear();
			roster_col_count = 0;
			roster_row_count++;
		}
	}
	
	// NOTE(juha): If the last row couldn't be filled completely:
	if (roster_col_count > 0) {
		knights.push_back(roster_tmp_row);
		roster_row_count++;
	}
	
	// NOTE(juha): Go through the levels.xml and parse the data.
	level_result = level_doc.load_file("../Scripts/levels.xml");

	for(pugi::xml_node_iterator iterator = level_doc.child("levels").begin();
								iterator != level_doc.child("levels").end();
							  ++iterator) {
		Sprite *tmp_sprite;
		SDL_Point tmp_point;
		level_paths.push_back(iterator->attribute("script").value());
		level_tilesets.push_back(iterator->attribute("tileset").value());
		tmp_point.x = atoi(iterator->attribute("start_x").value());
		tmp_point.y = atoi(iterator->attribute("start_y").value());
		start_points.push_back(tmp_point);

		tmp_sprite = new Sprite(window, 
			iterator->attribute("preview").value(), 80, 45);

		level_sprites.push_back(tmp_sprite);
	}

	// NOTE(juha): Create the player controllers.
	for (int i = 0; i < max_players; i++) {
		playerControllers.push_back(new PlayerController(multiplayer, 
														 i, 
														 &positions));
	}
	
	for (int i = 0; i < active_players; i++) {
		playerControllers[i]->in_game = true;
	}

	// NOTE(juha): If there are gamepads connected to the PC,
	// assign them to the 4th and 3rd player, respectively.
	if (controller_index == 2) {
		playerControllers[2]->setGamepad(ControllerHandles[0]);
		playerControllers[3]->setGamepad(ControllerHandles[1]);
	} else if (controller_index == 1) {
		playerControllers[3]->setGamepad(ControllerHandles[0]);
	} else if (controller_index == 0) {
		// nobody gamepads rip
	}
	
	// NOTE(juha): Initialize the menu positions.
	for (int i = 0; i < max_players; i++) {
		SDL_Point tmp;
		tmp.x = 0;
		tmp.y = 0;
		positions.push_back(tmp);
	}

	for (int i = 0; i < max_players; i++) {
		positions[i].x = playerControllers[i]->menu_x;
		positions[i].y = playerControllers[i]->menu_y;
		playerControllers[i]->setPlayers(&active_players);
	}
}

void CharacterSelectState::load(StateData *data) {}

stateStatus CharacterSelectState::update() 
{
	stateStatus status;
	status.status = STATE_CONTINUE;
	status.prepend = false;

		if (mainInput->keyPressed(SDL_SCANCODE_RETURN))	{
			if (character_select) {
				// NOTE(juha): Move into level select where only the first
				// player can control the selection.
				character_select = false;
				level_select = true;
				playerControllers[0]->menu_x = 0;
				playerControllers[0]->menu_y = 0;
			} else if (level_select) {
				sfx_select1.play(1);
				status.status = STATE_GAME;
			}
		}	

		if(mainInput->keyPressed(SDL_SCANCODE_ESCAPE)) {
			status.status = STATE_QUIT;
		}
		
	for (int i = 0; i < max_players; i++) {
		playerControllers[i]->update();
	}
	
	// NOTE(juha): If a player joins the game, resize the active players.
	stateData->selection.resize(active_players);
	
	// NOTE(juha): For checking if the selections of the players collide.
	bool intruder = false;

	// NOTE(juha): Checks if the active players already in the game are in the
	// positions of the grid where a new player would spawn if they joined the
	// game.
	for (int i = 0; i < max_players; i++) {
		if (playerControllers[i]->in_game == false) {
			for (int j = 0; j < max_players; j++) {
				if (playerControllers[i]->menu_x == positions[j].x &&
					playerControllers[i]->menu_y == positions[j].y &&
					i != j) {
						// NOTE(juha): If they are in the same position, then 
						// go through the first four spots in the grid and
						// switch to a free spot.
						for (int k = 0; k < 4; k ++) {
							bool filled = false;
							for (int l = 0; l < max_players; l++) {
								if (playerControllers[l]->menu_x == k) {
									filled = true;
									break;
								}
							}
							if (filled == false) {
								playerControllers[i]->menu_x = k;
								break;
							}
						}
					positions[i].x = playerControllers[i]->menu_x;
				}
			}
		}
	}
	
	menuMovement();

	// NOTE(juha): Commit the movements of the players.
	for (int i = 0; i < active_players; i++) {
		positions[i].x = playerControllers[i]->menu_x;
		positions[i].y = playerControllers[i]->menu_y;
	}
	return status;
}

void CharacterSelectState::menuMovement()
{
	if (character_select) {
		// NOTE(juha): For checking if the selections of the players collide.
		bool intruder = false;
		for (int i = 0; i < active_players; i++) {
			int loop_guard = 0;
			do {
				for (int j = 0; j < (int)positions.size(); j++) {

					// NOTE(juha): Check if a player overlaps with another 
					// player and keep moving them to the selected direction as
					// long as necessary.
					if (playerControllers[i]->menu_x == positions[j].x &&
						playerControllers[i]->menu_y == positions[j].y &&
						playerControllers[i]->in_game == true &&
						playerControllers[j]->in_game == true &&
						i != j) {

						if (playerControllers[i]->menu_x > positions[i].x) {
							playerControllers[i]->menu_x += 1;
						} else if (playerControllers[i]->menu_x < positions[i].x) {
							playerControllers[i]->menu_x -= 1;
						} else if (playerControllers[i]->menu_y > positions[i].y) {
							playerControllers[i]->menu_y += 1;
						} else if (playerControllers[i]->menu_y < positions[i].y) {
							playerControllers[i]->menu_y -= 1;
						}
						intruder = true;
						break;
					} else {
						intruder = false;
					}
				}
				loop_guard++;

				// NOTE(juha): Failsafe for a situation where the players move
				// to the same spot at exactly the same time so that the game
				// won't freeze.
				if (loop_guard >= 120) {
					for (int j = 0; j < max_players; ++j) {
						playerControllers[i]->menu_x = i;
					}
					break;
				}

			} while (intruder == true);
		}

		// NOTE(juha): If a player tries to move outside the grid, nullify the
		// move attempt.
		for (int i = 0; i < active_players; i++) {
			if (playerControllers[i]->menu_x >= 0 &&
				playerControllers[i]->menu_x < (int)knights[0].size() &&
				playerControllers[i]->menu_y >= 0 &&
				playerControllers[i]->menu_y < (int)knights.size()) {
			
				if (playerControllers[i]->menu_x > 
					(int)knights[playerControllers[i]->menu_y].size() - 1) {
					playerControllers[i]->menu_x = positions[i].x;
					playerControllers[i]->menu_y = positions[i].y;
				} else {
					positions[i].x = playerControllers[i]->menu_x;
					positions[i].y = playerControllers[i]->menu_y;
				}
			} else {
				playerControllers[i]->menu_x = positions[i].x;
				playerControllers[i]->menu_y = positions[i].y;
			}
		}
	} else if (level_select) {
		int level_rows = 1;

		if (playerControllers[0]->menu_x >= 0 &&
			playerControllers[0]->menu_x < (int)level_paths.size() &&
			playerControllers[0]->menu_y >= 0 &&
			playerControllers[0]->menu_y < level_rows) {
			
			if (playerControllers[0]->menu_x > 
				(int)knights[playerControllers[0]->menu_y].size() - 1) {
				playerControllers[0]->menu_x = positions[0].x;
				playerControllers[0]->menu_y = positions[0].y;
			} else {
				positions[0].x = playerControllers[0]->menu_x;
				positions[0].y = playerControllers[0]->menu_y;
			}
		} else {
			playerControllers[0]->menu_x = positions[0].x;
			playerControllers[0]->menu_y = positions[0].y;
		}

		// NOTE(juha): Save selected data.
		selected_level = level_paths[positions[0].x];
		selected_tileset = level_tilesets[positions[0].x];
		selected_start = start_points[positions[0].x];
	}
}

void CharacterSelectState::render() 
{
	if (character_select) {
		header->print(window, "Choose your destiny",
					  170, (int)(MARGIN_TOP / 2));

		// NOTE(juha): Go through the roster and render deselected and selected
		// versions of the knight decals based on the players' selection.
		for (int i = 0; i < roster_row_count; i++) {
			for (int j = 0; j < (int)knights[i].size(); ++j) {	
				if (knights[i][j]) {

					for (int k = 0; k < active_players; k++) {
						if (j == positions[k].x && i == positions[k].y) {
							knights[i][j]->getDecal()->setIndex(0);
							stateData->selection[k] = knights[i][j];
							break;
						} else {
							knights[i][j]->getDecal()->setIndex(1);
						}
					}

					knights[i][j]->getDecal()->render(MARGIN_LEFT + 30 * j, 
													  MARGIN_TOP  + 30 * i);
				}
			}
		}
	
		// NOTE(juha): Render player numbers over the selected decals.
		for (int i = 0; i < active_players; i++)
		{
			tag_shadows[i]->print(window, tag_texts[i], 
				MARGIN_LEFT + 17 + positions[i].x * 30,
				MARGIN_TOP  + 3  + positions[i].y * 30);
		
			tags[i]->print(window, tag_texts[i],
				MARGIN_LEFT + 18 + positions[i].x * 30,
				MARGIN_TOP  + 2  + positions[i].y * 30);
		}
		
		// NOTE(juha): Render the names of the knights next to the respective
		// player. If some players are not active "PRESS ACTION" will be 
		// printed instead of the knight name.
		for (int i = 0; i < max_players; i++)
		{
			player_identifiers[i]->print(window, id_texts[i],
				id_positions[i].x,
				id_positions[i].y);

			if (playerControllers[i]->in_game == true) {
				player_names[i]->print(window, 
									   stateData->selection[i]->getTruename(),
									   id_positions[i].x,
									   id_positions[i].y + 15);
			} else {
				player_names[i]->print(window, "PRESS ACTION",
					id_positions[i].x,
					id_positions[i].y + 15);
			}
		}
	} else if (level_select) {
		header->print(window, "Choose your might", 170, (int)(MARGIN_TOP / 2));
	
		// NOTE(juha): Render the level thumbnails.
		for (unsigned int k = 0; k < level_paths.size(); k++) {
			if (k == positions[0].x && 0 == positions[0].y) {
				level_sprites[k]->setIndex(0);
			} else {
				level_sprites[k]->setIndex(1);
			}
			
			level_sprites[k]->render(MARGIN_LEFT + 80 * k,
									 MARGIN_TOP  + 45 * 0);
		}
	}
}

StateData *CharacterSelectState::getStateData()
{
	stateData->players = active_players;
	for (int i = 0; i < controller_index; ++i) {
		stateData->ControllerHandles[i] = ControllerHandles[i];
	}
	stateData->controller_index = controller_index;
	stateData->lives = lives_total;
	stateData->level_path.assign(selected_level);
	stateData->level_tileset.assign(selected_tileset);
	stateData->start_x = selected_start.x;
	stateData->start_y = selected_start.y;
	stateData->knights = &knights;
	return stateData;
}
