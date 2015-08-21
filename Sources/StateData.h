/**
 * StateData.h
 *
 */

#ifndef __STATEDATA_H_DEFINED__
#define __STATEDATA_H_DEFINED__

#include "Knight.h"

#define MAX_CONTROLLERS 2

class StateData
{
public:
	StateData();

	// NOTE(juha): Filled but not really used at the moment.
	std::vector<std::vector<Knight *> > *knights;

	std::vector<Knight *> selection;
	int players;
	std::string level_path;
	std::string level_tileset;
	int start_x, start_y;
	std::vector<int> player_kills;
	std::vector<int> player_deaths;
	int lives, ControllerIndex;
	SDL_GameController *ControllerHandles[MAX_CONTROLLERS];

private:
};

#endif // __STATEDATA_H_DEFINED__