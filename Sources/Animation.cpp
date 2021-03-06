/**
 * Animation.cpp
 *
 */


#include "Animation.h"

Animation::Animation(Window *window, std::string filename,
					 int width, int height, int start_frame,
					 int frame_count, int framerate):
					 Sprite(window, filename, width, height),
					 window(window),
					 times_played(0),
					 running(true),
					 play_count(0),
					 frametime(0),
					 current_frame(0),
					 current_frametime(0)
{
	setFramerate(framerate);

	for (int frame = 0; frame < frame_count; ++frame) {
		frames.push_back(frame);
	}
}

Animation::~Animation() {}

void Animation::setFramerate(int framerate)
{
	frametime = (1000 / framerate);
}

void Animation::setCurrentFrame(int i)
{
	current_frame = i;
}

int Animation::getCurrentFrame()
{
	return current_frame;
}

int Animation::getFramecount()
{
	return frames.size();
}

void Animation::play(int times)
{
	running = true;
	play_count = times; 
}

void Animation::pause()
{
	running = false;
}

void Animation::nextFrame()
{
	current_frame++;
}

void Animation::render(int x, int y)
{
	current_frametime += window->getDelta();

	if (running && play_count != 0) {
		if (current_frametime > frametime) {
			if (current_frame >= (int)frames.size() - 1) {
				play_count--;
				times_played++;

				if (play_count == 0) {
					running = false;
				}

				current_frame = 0;
			} else {
				current_frame++;
			}

			current_frametime = 0;
		}
	}

	Sprite::setIndex(current_frame);

	if (current_frame != 0 || running) {
		Sprite::render(x, y);
	}
}

bool Animation::done()
{
	if (running && current_frame == frames.size() - 1) {
		return true;
	}

	return false;
}