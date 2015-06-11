/**
 * Music.cpp
 * 
 *
 */

#include "Music.h"

Music::Music(std::string filepath):
	music(nullptr),
	filepath(filepath)
{
	music = Mix_LoadMUS(this->filepath.c_str());

	if (!music) {
		printf("Audio error! Here it is: %s", Mix_GetError());
		SDL_Delay(5000);
	}
}

Music::~Music()
{
    if (music) {
	    Mix_FreeMusic(this->music);
	}
}

bool Music::play(int times, int fadeInMs)
{
	if (!music) {
        return false;
	}

    if (Mix_FadeInMusic(music, times, fadeInMs)) {
	    printf("Mix_FadeInMusic: Couldn't play music on %s", filepath.c_str());

	    return false;
    }
	
    return true;
}
void Music::fadeOut(int ms)
{
	Mix_FadeOutMusic(ms);
}

void Music::stop()
{
	Mix_HaltMusic();
}

void Music::pause()
{
	Mix_PauseMusic();
}

void Music::unpause()
{
	Mix_ResumeMusic();
}

void Music::restart()
{
    stop();
    play();
}

void Music::rewind()
{
	Mix_RewindMusic();
}

void Music::setPosition(double seconds)
{
	rewind();
	if (Mix_SetMusicPosition(seconds) == -1) {
		printf("Mix_SetMusicPosition: Couldn't do that: %s", std::string(Mix_GetError()).c_str());
	}
}
bool Music::isPlaying()
{
	// Note: Mix_PlayingMusic only tells if the music started
	//       playing.
	//       Even if it's paused it will return true!

	return (Mix_PlayingMusic() && !isPaused());
}
bool Music::isPaused()
{
	return (bool)Mix_PausedMusic();
}
bool Music::isFadingIn()
{
	return (Mix_FadingMusic() == MIX_FADING_IN);
}
bool Music::isFadingOut()
{
	return (Mix_FadingMusic() == MIX_FADING_OUT);
}
int Music::setVolume(int volume)
{
	if (volume > MIX_MAX_VOLUME)
		printf("Mix_VolumeMusic: Tried to overflow volume");

	return Mix_VolumeMusic(volume);
}
int Music::getVolume()
{
	return Mix_VolumeMusic(-1);
}
int Music::getMaxVolume()
{
	return MIX_MAX_VOLUME;
}