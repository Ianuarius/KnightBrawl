/**
 * CharacterSelectState.cpp
 *
 */

#include "CharacterSelectState.h"

CharacterSelectState::CharacterSelectState(Window *window, Input *mainInput):
	window(window),
	mainInput(mainInput),
	font(new Font("ChicagoFLF.ttf", 16)),
	header(new Text(font, Color("white")))
{
	stateData = new StateData();
	bool multiplayer = true;
	players = 2;
	stateData->players = players;
	stateData->selection.reserve(players);
	for (int i = 0; i < players; i++)
	{
		stateData->selection.push_back(nullptr);
	}

	/*
		TODO(juha): Go thru the roster
		- for every row make new knight with that id
		- 
	*/

	roster_result = roster_document.load_file("Scripts/roster.xml");
	
	row_length = 3;
	row_count = 1;
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
		while (col_count % row_length != 0) {
			tmp_row.push_back(nullptr);
			col_count++;
		}

		knights.push_back(tmp_row);
		tmp_row.clear();
		col_count = 0;
	}

	// TODO(juha): Difference for initialized players and playing players.
	for (int i = 0; i < players; i++) {
		playerControllers.push_back(new PlayerController(multiplayer, i));
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

	return status;
}

void CharacterSelectState::render() 
{
	header->print(window, "Choose your destiny", 170, (int)(MARGIN_TOP / 2));
	
	for (int i = 0; i < row_count; i++)
	{
		for (int j = 0; j < row_length; j++)
		{	
			if (knights[i][j] != nullptr) {

				for (int k = 0; k < players; k++) {
					if (j == playerControllers[k]->menu_x && i == playerControllers[k]->menu_y) {
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
}

StateData *CharacterSelectState::getStateData()
{
	stateData->knights = &knights;
	return stateData;
}
