/**
 * StateData.cpp
 *
 */

#include "StateData.h"

StateData::StateData():
	players(0),
	level_path(""),
	level_tileset(""),
	start_x(0), start_y(0)
{
	player_deaths.resize(4);
	player_kills.resize(4);
}