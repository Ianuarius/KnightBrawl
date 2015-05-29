/**
 * Animation.h
 *
 */

#ifndef __ANIMATION_H_INCLUDED__
#define __ANIMATION_H_INCLUDED__

#include <vector>
#include "SDL.h"
#include "Sprite.h"

#define INFINITE_LOOP -1

class Animation : public Sprite
{
	public:
		Animation(Window *window, std::string filename, int width, int height,
				  int start_frame, int frame_count, int framerate);
		virtual ~Animation();

		void addFrame(int frame_index);
		void setFramerate(int framerate);

		int getFramecount();
		void setCurrentFrame(int i);
		int getCurrentFrame();

		void play(int times);
		void pause();
		void nextFrame();
		void render(int x, int y);
		bool done();

		bool running;
		int times_played;

	private:
		Window *window;

		std::vector<int> frames;
		int play_count, frametime, current_frame, current_frametime;
};

#endif // __ANIMATION_H_INCLUDED__