#ifndef I_AUDIO_PROVIDER
#define I_AUDIO_PROVIDER

#include <string>

class IAudioProvider
{
public:
	~IAudioProvider() {};

	virtual void playSound(std::string filename) = 0;
	virtual void playSong(std::string filename, bool looping) = 0;
	virtual void stopAllSound() = 0;

	virtual bool isSoundPlaying() = 0;
	virtual bool isSongPlaying() = 0;
};

#endif