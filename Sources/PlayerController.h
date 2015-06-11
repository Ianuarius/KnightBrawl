/**
 * PlayerController.h
 *
 */

#ifndef __PLAYERCONTROLLER_H_DEFINED__
#define __PLAYERCONTROLLER_H_DEFINED__

#include "Input.h"
#include "SDL.h"
#include "PugiXML.h"
#include "Keymap.h"
#include <string>

enum Direction {RIGHT, LEFT};

class PlayerController
{
public:
	PlayerController(SDL_Point start_position, bool multiplayer, int player);
	SDL_Point getLocation();
	int getDirection();
	
	void update();
	
private:
	EnumParser<SDL_Scancode> fieldTypeParser;
	pugi::xml_document controlsDocument;
	pugi::xml_parse_result result;
	pugi::xml_node SinglePlayerMappings;
	pugi::xml_node MultiPlayerMappings;
	SDL_Point location;
	bool multiplayer;
	int facing;

};

#endif //__PLAYERCONTROLLER_H_DEFINED__