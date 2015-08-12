/**
 * VictoryState.h
 *
 */

#ifndef __VICTORYSTATE_H_DEFINED__
#define __VICTORYSTATE_H_DEFINED__

#include "BaseState.h"
#include "Color.h"
#include "Font.h"
#include "Input.h"
#include "StateManager.h"
#include "Text.h"
#include "Texture.h"
#include "Timer.h"
#include "Window.h"

#define SLIDE_TIME 2000

class VictoryState : public BaseState 
{
public:
	VictoryState(Window *window, Input *mainInput);
	~VictoryState();
	stateStatus update();
	StateData *getStateData();
	void load(StateData *data);
	void render();

private:
	Font *title;
	Font *values;

	std::vector<Text *> names;
	std::vector<Text *> kills;
	std::vector<Text *> deaths;
	std::vector<int> player_rank;
	Text *knight_wins,
		*kills_text, *deaths_text;

	Window *window;
	Input *mainInput;
	Text slideShowText();
	// Texture victory;
	int slide_index, winner;
	Timer timer;
	StateData *stateData;
};

#endif // __VICTORYSTATE_H_DEFINED__