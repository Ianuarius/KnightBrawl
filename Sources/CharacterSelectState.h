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

#define MARGIN_TOP	100
#define MARGIN_LEFT	200

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

	Font *font;
	Font *player_tag;
	Text *header, *p1_text, *p2_text, *p3_text;
	Input *mainInput;
	Window *window;
	GameState *game;
	int players, row_length, row_count;
	bool start;
	bool gameover;
	StateData *stateData;
	std::vector<SDL_Point> positions;
	std::vector<Text *> tags;
	std::vector<Text *> tag_shadows;
	std::vector<std::string> tag_texts;
	std::vector<std::vector<Knight *> > knights;
	std::vector<PlayerController *> playerControllers;
	pugi::xml_parse_result knight_result, roster_result;
	pugi::xml_document knight_document, roster_document;
};

#endif // __CHARACTERSELECTSTATE_H_DEFINED__