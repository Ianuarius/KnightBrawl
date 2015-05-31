/**
 * Scorestate.cpp
 *
 */

#include "ScoreState.h"

ScoreState::ScoreState(Window *window) :
window(window),
background(window, ""),
font(new Font("", 28)),
playername(new Text(font, Color("white")))
{

}

ScoreState::~ScoreState()
{
	delete window;
}

void ScoreState::render() {
	background.render(0, 0);
	playername->print(window, "", 40, 25);
}
