/**
 * CharacterSelectState.h
 *
 */

#ifndef __CHARACTERSELECTSTATE_H_DEFINED__
#define __CHARACTERSELECTSTATE_H_DEFINED__

#include <stdio.h>
#include "BaseState.h"
#include "Color.h"
#include "Font.h"
#include "GameState.h"
#include "Input.h"
#include "Music.h"
#include "PlayerController.h"
#include "Text.h"
#include "Sprite.h"
#include "SDL.h"

#define MARGIN_TOP	100
#define MARGIN_LEFT	200
#define MAX_CONTROLLERS 2

class CharacterSelectState: public BaseState 
{
public:
	CharacterSelectState(Window *window, Input *mainInput);
	void render();
	stateStatus update();
	StateData *getStateData();
	void load(StateData *data);

private:
	void menuMovement();

	Font *font, *playerfont;
	Font *player_tag;
	Text *header, *p1_text, *p2_text, *p3_text;
	Input *mainInput;
	Window *window;
	GameState *game;
	int active_players, max_players, roster_row_length, roster_row_count, lives_total;
	int ControllerIndex;
	bool start, gameover, character_select, level_select;
	StateData *stateData;
	std::string selected_level;
	std::string selected_tileset;
	SDL_Point selected_start;

	SDL_GameController *ControllerHandles[MAX_CONTROLLERS];
	std::vector<PlayerController *> playerControllers;

	std::vector<SDL_Point> positions;
	std::vector<SDL_Point> id_positions;
	std::vector<SDL_Point> start_points;

	std::vector<Text *> tags;
	std::vector<Text *> tag_shadows;
	std::vector<Text *> player_identifiers;
	std::vector<Text *> player_names;

	std::vector<std::string> tag_texts;
	std::vector<std::string> id_texts;
	std::vector<std::string> level_paths;
	std::vector<std::string> level_tilesets;

	std::vector<std::vector<Knight *> > knights;
	std::vector<Sprite *> level_sprites;
	
	pugi::xml_parse_result knight_result, roster_result, level_result;
	pugi::xml_document knight_document, roster_document, level_document;
};

#endif // __CHARACTERSELECTSTATE_H_DEFINED__