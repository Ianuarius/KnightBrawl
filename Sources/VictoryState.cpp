/**
 * VictoryState.cpp
 *
 */

#include "VictoryState.h"

VictoryState::VictoryState(Window *window, Input *mainInput):
	window(window),
	mainInput(mainInput),
	title(new Font("../Fonts/ChicagoFLF.ttf", 24)),
	values(new Font("../Fonts/ChicagoFLF.ttf", 16)),
	knight_wins(new Text(title, Color(0x48B748))),
	kills_text(new Text(values, Color(0xDEDEA6))),
	flawless_text(new Text(values, Color(0xFF3333))),
	deaths_text(new Text(values, Color(0xDEDEA6)))
{	
	timer.start();
	player_rank.resize(4);
	flawless = false;
}

VictoryState::~VictoryState() 
{
}

void VictoryState::load(StateData *data)
{
	stateData = data;
	
	// NOTE(juha): Sorting functionality.
	for (int i = 0; i < stateData->players; i++) {
		for (int j = 0; j < stateData->players; j++) {
			if (i != j) {
				if (stateData->player_kills[i] < stateData->player_kills[j]) {
					player_rank[i]++;
				} else if (stateData->player_kills[i] == stateData->player_kills[j] &&
					stateData->player_deaths[i] > stateData->player_deaths[j]) {
					player_rank[i]++;
				}
			}
		}
	}
	
	for (int i = 0; i < stateData->players; i++) {
		bool not_ok = false;

		for (int j = 0; j < stateData->players; j++) {
			if (player_rank[j] == i) {
				not_ok = false;
				break;
			} else {
				not_ok = true;
			}
		}

		if (not_ok) {
			int counter = 0;
			for (int j = 0; j < stateData->players; j++) {
				if (player_rank[j] == i - 1) {
					if (counter > 0) {
						player_rank[j]++;
					} else {
						counter++;
					}
				}
			}
		}
	}

	// NOTE(juha): With value 9999 there's no defined winner.
	winner = 9999;

	for (int i = 0; i < stateData->players; i++) {
		// NOTE(juha): If the match ended with a player who didn't lose all of
		// their lives, then that player is the winner of the round.
		if (stateData->player_deaths[i] < stateData->lives) {
			names.push_back(new Text(values, Color(0x48B748)));
			kills.push_back(new Text(values, Color(0x48B748)));
			deaths.push_back(new Text(values, Color(0x48B748)));
			winner = i;
			if (stateData->player_deaths[i] == 0) {
				flawless = true;
			}
		} else {
			names.push_back(new Text(values, Color(0xB60D0D)));
			kills.push_back(new Text(values, Color(0xB60D0D)));
			deaths.push_back(new Text(values, Color(0xB60D0D)));
		}
	}
}

stateStatus VictoryState::update()
{
	stateStatus status;
	status.status = STATE_CONTINUE;
	status.prepend = false;
	
	if (mainInput->keyPressed(SDL_SCANCODE_RETURN) || 
		mainInput->keyPressed(SDL_SCANCODE_ESCAPE) || 
		mainInput->keyPressed(SDL_SCANCODE_SPACE))	{
		status.status = STATE_MENU;
		return status;
	}

	return status;
}

void VictoryState::render() 
{
	if (winner == 9999) {
		knight_wins->print(window, "It's a TIE!", 135, 70);
	} else {
		knight_wins->print(window, 
			stateData->selection[winner]->getTruename() + " Wins!", 135, 70);
	}

	if (flawless) {
		flawless_text->print(window, "Flawless Victory", 155, 95);
	}

	kills_text->print(window, "Kills", 175, 120);
	deaths_text->print(window, "Deaths", 280, 120);

	for (int i = 0; i < stateData->players; i++) {
		for (int j = 0; j < stateData->players; j++) {
			if (player_rank[j] == i) {
				names[j]->print(window, 
					stateData->selection[j]->getTruename(), 
					70, 135 + (15 * i));

				kills[j]->print(window, 
					to_string(stateData->player_kills[j]), 
					190, 135 + (15 * i));

				deaths[j]->print(window, 
					to_string(stateData->player_deaths[j]), 
					305, 135 + (15 * i));
			}
		}
	}
}

StateData *VictoryState::getStateData()
{
	return nullptr;
}
