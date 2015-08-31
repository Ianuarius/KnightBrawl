/**
 * Animation.h
 * 
 * Handles the definition and playback of frame-based animations.
 * 
 */

#ifndef ANIMATION_H_INCLUDED
#define ANIMATION_H_INCLUDED

#include <vector>
#include "SDL.h"
#include "Sprite.h"

static const int INFINITE_LOOP = -1;

class Animation : public Sprite
{
public:
	// Parameters:
		// window		Pointer to the Window object created in main.cpp.
		// filename		The path and filename to the image file.
		// width		The width of a single frame of the animation.
		// height		The height of a single frame of the animation.
		// start_frame	The point where the animation should start.
		// frame_count	The number of frames in the animation.
		// framerate	The amount of frames displayed during one second.
	Animation(Window *window, std::string filename, int width, int height,
				int start_frame, int frame_count, int framerate);
	virtual ~Animation();

	// Sets the framerate of the animation to the given framerate.
	void setFramerate(int framerate);

	// Switches the current frame of the animation to be i.
	void setCurrentFrame(int i);

	int getCurrentFrame();
	int getFramecount();

	void play(int times);
	void pause();
	void nextFrame();

	void render(int x, int y);

	// Checks if the animation has done running through.
	bool done();

	bool running;
	int times_played;

private:
	Window *window;

	std::vector<int> frames;
	int play_count, frametime, current_frame, current_frametime;
};

#endif // ANIMATION_H_INCLUDED