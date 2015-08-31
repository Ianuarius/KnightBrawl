/**
 * CharacterSelectState.h
 *
 * State where the knights and the level are selected by the players.
 *
 */

#ifndef CHARACTERSELECTSTATE_H_DEFINED
#define CHARACTERSELECTSTATE_H_DEFINED

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
#include "SFX.h"

#define MARGIN_TOP			100
#define MARGIN_LEFT			200

#define MAX_CONTROLLERS		2

class CharacterSelectState: public BaseState 
{
public:
	// Parameters:
		// window		Pointer to the Window object created in main.cpp.
		// mainInput	Pointer to the Input object created in main.cpp.
	CharacterSelectState(Window *window, Input *mainInput);
	void render();
	stateStatus update();
	
	// Assign data into a stateData object and return it.
	StateData *getStateData();

	// Not used here since there's currently nothing to get from MenuState.    
	void load(StateData *data);

private:
	// Checks the updated menu positions of the players and makes necessary
	// changes to them so that none of the positions overlap.
	void menuMovement();

	Font *font, *playerfont;
	Font *player_tag;
	GameState *game;
	Input *mainInput;
	SDL_GameController *ControllerHandles[MAX_CONTROLLERS];
	SDL_Point selected_start;
	SFX sfx_select1;
	StateData *stateData;
	Text *header, *p1_text, *p2_text, *p3_text;
	Window *window;
	
	std::string selected_level;
	std::string selected_tileset;

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
	pugi::xml_document knight_document, roster_doc, level_doc;
	
	int active_players, max_players, roster_row_length, roster_row_count;
	int lives_total;
	int controller_index;
	bool start, gameover, character_select, level_select;
};

#endif // CHARACTERSELECTSTATE_H_DEFINED