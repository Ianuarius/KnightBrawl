/**
 * StateData.h
 *
 * Class for data that needs to be transported between states.
 *
 */

#ifndef STATEDATA_H_DEFINED
#define STATEDATA_H_DEFINED

#include "Knight.h"

#define MAX_CONTROLLERS 2

class StateData
{
public:
	StateData();
	
	SDL_GameController *ControllerHandles[MAX_CONTROLLERS];

	std::string level_path;
	std::string level_tileset;

	std::vector<Knight *> selection;
	std::vector<int> player_kills;
	std::vector<int> player_deaths;
	
	// TODO(juha): Filled but not really used at the moment.
	std::vector<std::vector<Knight *> > *knights;
	
	int players;
	int start_x, start_y;
	int lives, controller_index;

private:
};

#endif // STATEDATA_H_DEFINED