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
	bool multiplayer = true;
	character_select = true;
	level_select = false;
	active_players = 2;
	max_players = 4;
	lives_total = 3;
	stateData->selection.resize(active_players);
	
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


	roster_result = roster_document.load_file("../Scripts/roster.xml");
	level_result = level_document.load_file("../Scripts/levels.xml");

	
	for(pugi::xml_node_iterator iterator = level_document.child("levels").begin();
			iterator != level_document.child("levels").end();
			++iterator)
	{
		Sprite *tmp_sprite;
		SDL_Point tmp_point;
		level_paths.push_back(iterator->attribute("script").value());
		level_tilesets.push_back(iterator->attribute("tileset").value());
		tmp_point.x = atoi(iterator->attribute("start_x").value());
		tmp_point.y = atoi(iterator->attribute("start_y").value());
		start_points.push_back(tmp_point);

		tmp_sprite = new Sprite(window, iterator->attribute("preview").value(), 80, 45);

		level_sprites.push_back(tmp_sprite);
	}

	roster_row_length = 5;
	roster_row_count = 0;
	int roster_col_count = 0;
	std::vector<Knight *> roster_tmp_row;

	for(pugi::xml_node_iterator iterator = roster_document.child("roster").begin();
			iterator != roster_document.child("roster").end();
			++iterator)
	{
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
	
	if (roster_col_count > 0) {
		knights.push_back(roster_tmp_row);
		roster_row_count++;
	}

	for (int i = 0; i < max_players; i++) {
		SDL_Point tmp;
		tmp.x = 0;
		tmp.y = 0;
		positions.push_back(tmp);
	}

	// TODO(juha): Difference for initialized players and playing players.
	for (int i = 0; i < max_players; i++) {
		playerControllers.push_back(new PlayerController(multiplayer, 
														 i, 
														 &positions));
	}
	
	for (int i = 0; i < max_players; i++) {
		positions[i].x = playerControllers[i]->menu_x;
		positions[i].y = playerControllers[i]->menu_y;
		playerControllers[i]->setPlayers(&active_players);
	}
	
	for (int i = 0; i < active_players; i++) {
		playerControllers[i]->in_game = true;
	}
}

void CharacterSelectState::load(StateData *data)
{
	
}

stateStatus CharacterSelectState::update() 
{
	stateStatus status;
	status.status = STATE_CONTINUE;
	status.prepend = false;

		if (mainInput->keyPressed(SDL_SCANCODE_RETURN))	{
			if (character_select) {
				character_select = false;
				level_select = true;
				playerControllers[0]->menu_x = 0;
				playerControllers[0]->menu_y = 0;
			} else if (level_select) {
				status.status = STATE_GAME;
			}
		}	

		if(mainInput->keyPressed(SDL_SCANCODE_ESCAPE)) {
			status.status = STATE_QUIT;
		}
		
	for (int i = 0; i < max_players; i++) {
		playerControllers[i]->update();
	}
	
	stateData->selection.resize(active_players);
	
	bool intruder = false;

	for (int i = 0; i < max_players; i++) {
		
		if (playerControllers[i]->in_game == false) {

			for (int j = 0; j < max_players; j++) {

				if (playerControllers[i]->menu_x == positions[j].x &&
					playerControllers[i]->menu_y == positions[j].y &&
					i != j) {
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

	for (int i = 0; i < active_players; i++) {
		positions[i].x = playerControllers[i]->menu_x;
		positions[i].y = playerControllers[i]->menu_y;
	}
	return status;
}

void CharacterSelectState::menuMovement()
{
	if (character_select) {
		bool intruder = false;
		for (int i = 0; i < active_players; i++) {
		
			do {
				for (int j = 0; j < (int)positions.size(); j++) {

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
			} while (intruder == true);
		}

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
		bool intruder = false;
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

		selected_level = level_paths[positions[0].x];
		selected_tileset = level_tilesets[positions[0].x];
		selected_start = start_points[positions[0].x];
	}
}

void CharacterSelectState::render() 
{
	if (character_select) {
		header->print(window, "Choose your destiny", 170, (int)(MARGIN_TOP / 2));
	
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
	
		for (int i = 0; i < active_players; i++)
		{
			tag_shadows[i]->print(window, tag_texts[i], 
				MARGIN_LEFT + 17 + positions[i].x * 30,
				MARGIN_TOP  + 3  + positions[i].y * 30);
		
			tags[i]->print(window, tag_texts[i],
				MARGIN_LEFT + 18 + positions[i].x * 30,
				MARGIN_TOP  + 2  + positions[i].y * 30);
			
			player_identifiers[i]->print(window, id_texts[i],
				id_positions[i].x,
				id_positions[i].y);
			
			player_names[i]->print(window, stateData->selection[i]->getTruename(),
				id_positions[i].x,
				id_positions[i].y + 15);
		}
	} else if (level_select) {
		header->print(window, "Choose your might", 170, (int)(MARGIN_TOP / 2));
	
		for (int k = 0; k < level_paths.size(); k++) {
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
	stateData->lives = lives_total;
	stateData->level_path.assign(selected_level);
	stateData->level_tileset.assign(selected_tileset);
	stateData->start_x = selected_start.x;
	stateData->start_y = selected_start.y;
	stateData->knights = &knights;
	return stateData;
}
