/**
 * VictoryState.h
 *
 * State for displaying the statistics after a match.
 *
 */

#ifndef VICTORYSTATE_H_DEFINED
#define VICTORYSTATE_H_DEFINED

#include "BaseState.h"
#include "Color.h"
#include "Font.h"
#include "Input.h"
#include "StateManager.h"
#include "Text.h"
#include "Texture.h"
#include "Timer.h"
#include "Window.h"

class VictoryState : public BaseState 
{
public:
	VictoryState(Window *window, Input *mainInput);
	~VictoryState();
	StateData *getStateData();

	// Loads the StateData object and based on the given kills and deaths
	// information ranks the players.
	void load(StateData *data);

	// Checks for input.
	stateStatus update();

	// Renders players' information on the screen in the order that they
	// were ranked in the load method.
	void render();

private:
	Font *title;
	Font *values;
	Input *mainInput;
	StateData *stateData;
	Text *knight_wins, *flawless_text,
		*kills_text, *deaths_text;
	Timer timer;
	Window *window;

	std::vector<Text *> names;
	std::vector<Text *> kills;
	std::vector<Text *> deaths;
	std::vector<int> player_rank;

	bool flawless;
	int slide_index, winner;
};

#endif // VICTORYSTATE_H_DEFINED