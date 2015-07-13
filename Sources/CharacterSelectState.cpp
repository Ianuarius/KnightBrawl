/**
 * CharacterSelectState.cpp
 *
 */

#include "CharacterSelectState.h"

CharacterSelectState::CharacterSelectState(Window *window, Input *mainInput):
	window(window),
	mainInput(mainInput),
	font(new Font("ChicagoFLF.ttf", 16)),
	player_tag(new Font("ChicagoFLF.ttf", 8)),
	header(new Text(font, Color("white")))
{
	stateData = new StateData();
	bool multiplayer = true;
	players = 3;
	stateData->players = players;
	stateData->selection.reserve(players);
	for (int i = 0; i < players; i++)
	{
		stateData->selection.push_back(nullptr);
	}
	
	for (int i = 0; i < players; i++)
	{
		tags.push_back(new Text(player_tag, Color("white")));
		tag_shadows.push_back(new Text(player_tag, Color("black")));
	}
	
	tag_texts.push_back("P1");
	tag_texts.push_back("P2");
	tag_texts.push_back("P3");
	tag_texts.push_back("P4");

	/*
		TODO(juha): Go thru the roster
		- for every row make new knight with that id
		- 
	*/

	roster_result = roster_document.load_file("Scripts/roster.xml");
	
	row_length = 4;
	row_count = 0;
	int col_count = 0;
	std::vector<Knight *> tmp_row;

	for(pugi::xml_node_iterator iterator = roster_document.child("roster").begin();
			iterator != roster_document.child("roster").end();
			++iterator)
	{
		col_count++;
		tmp_row.push_back(new Knight(window, atoi(iterator->attribute("id").value())));

		if (col_count >= row_length) {
			knights.push_back(tmp_row);
			tmp_row.clear();
			col_count = 0;
			row_count++;
		}
	}
	
	
	if (col_count > 0) {
		knights.push_back(tmp_row);
		row_count++;
	}
	/*
	if (col_count > 0) {
		while (col_count % row_length != 0) {
			tmp_row.push_back(nullptr);
			col_count++;
		}

		knights.push_back(tmp_row);
		tmp_row.clear();
		col_count = 0;
	}
	*/
	
	for (int i = 0; i < players; i++)
	{
		SDL_Point tmp;
		tmp.x = 0;
		tmp.y = 0;
		positions.push_back(tmp);
	}

	// TODO(juha): Difference for initialized players and playing players.
	for (int i = 0; i < players; i++) {
		playerControllers.push_back(new PlayerController(multiplayer, i, &positions));
	}
	
	for (int i = 0; i < players; i++)
	{
		positions[i].x = playerControllers[i]->menu_x;
		positions[i].y = playerControllers[i]->menu_y;
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
			status.status = STATE_GAME;
		}	

		if(mainInput->keyPressed(SDL_SCANCODE_ESCAPE)) {
			status.status = STATE_QUIT;
		}
		
	for (int i = 0; i < players; i++) {
		playerControllers[i]->update();
	}

	menuMovement();

	for (int i = 0; i < players; i++)
	{
		positions[i].x = playerControllers[i]->menu_x;
		positions[i].y = playerControllers[i]->menu_y;
	}


	return status;
}

void CharacterSelectState::menuMovement()
{
	bool intruder = false;
	for (int i = 0; i < players; i++) {
		
		do {
			for (int j = 0; j < positions.size(); j++) {

				if (playerControllers[i]->menu_x == positions[j].x &&
					playerControllers[i]->menu_y == positions[j].y &&
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
				} else {
					intruder = false;
				}
			}
		} while (intruder == true);
		

		if (playerControllers[i]->menu_x >= 0 &&
			playerControllers[i]->menu_x < knights[0].size() &&
			playerControllers[i]->menu_y >= 0 &&
			playerControllers[i]->menu_y < knights.size()) {
			
			if (playerControllers[i]->menu_x > knights[playerControllers[i]->menu_y].size() - 1) {
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
}


void CharacterSelectState::render() 
{
	header->print(window, "Choose your destiny", 170, (int)(MARGIN_TOP / 2));
	
	for (int i = 0; i < row_count; i++) {
		for (int j = 0; j < knights[i].size(); ++j) {	
			if (knights[i][j]) {

				for (int k = 0; k < players; k++) {
					if (j == positions[k].x && i == positions[k].y) {
						knights[i][j]->getDecal()->setIndex(0);
						stateData->selection[k] = knights[i][j];
						break;
					} else {
						knights[i][j]->getDecal()->setIndex(1);
					}
				}

				knights[i][j]->getDecal()->render(MARGIN_LEFT + 30 * j, MARGIN_TOP + 30 * i);
			}
		}
	}
	
	for (int i = 0; i < players; i++)
	{
		tag_shadows[i]->print(window, tag_texts[i], MARGIN_LEFT + 17 + positions[i].x * 30,
									                MARGIN_TOP  + 3  + positions[i].y * 30);
		
		tags[i]->print(window, tag_texts[i], MARGIN_LEFT + 18 + positions[i].x * 30,
									         MARGIN_TOP  + 2  + positions[i].y * 30);
	}
	
	/*
	p1_text->print(window, "P1", MARGIN_LEFT + 18 + positions[0].x * 30,
								 MARGIN_TOP  + 2 + positions[0].y * 30);
	p2_text->print(window, "P2", MARGIN_LEFT + 18 + positions[1].x * 30,
								 MARGIN_TOP  + 2 + positions[1].y * 30);
	p3_text->print(window, "P3", MARGIN_LEFT + 18 + positions[2].x * 30,
								 MARGIN_TOP  + 2 + positions[2].y * 30);
								 */

}

StateData *CharacterSelectState::getStateData()
{
	stateData->knights = &knights;
	return stateData;
}
