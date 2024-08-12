#ifndef SFML_SOUND_PROVIDER
#define SFML_SOUND_PROVIDER

#include "IAudioProvider.h"
#include "SFML/Audio.hpp"
#include "SoundFileCache.h"

class SFMLSoundProvider : public IAudioProvider
{
public:

	SFMLSoundProvider();

	void playSound(std::string filename);
	void playSong(std::string filename, bool looping);
	void stopAllSound();

	bool isSoundPlaying();
	bool isSongPlaying();

private:

	static const int MAX_SOUND_CHANNELS = 5;

	SoundFileCache m_soundFileCache{};
	sf::Sound m_currentSounds[MAX_SOUND_CHANNELS];
	std::string m_currentSongName;
};

#endif