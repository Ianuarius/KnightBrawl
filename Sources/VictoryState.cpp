/**
 * VictoryState.cpp
 *
 */


#include "VictoryState.h"

VictoryState::VictoryState(Window *window) :
window(window),
slideShowText(new Font("", 14), Color("white")),
slide_index(0) {
	
	backgrounds.push_back(new Texture(window, ""));
	texts.push_back("");

	
	backgrounds.push_back(new Texture(window, ""));
	texts.push_back("");

	backgrounds.push_back(new Texture(window, ""));
	texts.push_back("");


}

VictoryState::~VictoryState() {
}



void VictoryState::render() {
	backgrounds[slide_index]->render(0, 0);
	slideShowText.print(window, texts[slide_index], 60, 150);

}