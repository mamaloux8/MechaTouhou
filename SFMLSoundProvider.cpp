#include "SFMLSoundProvider.h"
#include "SoundFileCache.h"

SFMLSoundProvider::SFMLSoundProvider()
	: m_currentSongName{ "" }
{
}

void SFMLSoundProvider::playSound(std::string filename)
{
	int availChannel = -1;
	for (int i{}; i < MAX_SOUND_CHANNELS; ++i)
	{
		if (m_currentSounds[i].getStatus() != sf::Sound::Playing)
		{
			availChannel = i;
			break;
		}
	}

	//If all channels are in use, do nothing for now
	if (availChannel != -1)
	{
		try
		{
			m_currentSounds[availChannel] = m_soundFileCache.getSound(filename);
			m_currentSounds[availChannel].play();
		}
		catch (SoundNotFoundException&)
		{
			//ERROR, file wasn't found, should handle error here
		}
	}
}

void SFMLSoundProvider::playSong(std::string filename, bool looping)
{
	sf::Music* currentSong;
	try
	{
		currentSong = m_soundFileCache.getSong(filename);
	}
	catch (SoundNotFoundException&)
	{
		//Couldn't find or load the song
		return;
	}

	if (m_currentSongName != "")
	{
		try
		{
			sf::Music* priorSong = m_soundFileCache.getSong(m_currentSongName);
			if (priorSong->getStatus() != sf::Sound::Stopped)
			{
				priorSong->stop();
			}
		}
		catch (SoundNotFoundException&)
		{
			//Previous song wasn't located, it's not dire
		}
	}

	m_currentSongName = filename;
	currentSong->setLoop(looping);
	currentSong->play();
}

void SFMLSoundProvider::stopAllSound()
{
	for (int i{}; i < MAX_SOUND_CHANNELS; ++i)
	{
		m_currentSounds[i].stop();
	}

	if (m_currentSongName != "")
	{
		sf::Music* currentSong = m_soundFileCache.getSong(m_currentSongName);
		if (currentSong->getStatus() == sf::Sound::Playing)
		{
			currentSong->stop();
		}
	}
}

bool SFMLSoundProvider::isSoundPlaying()
{
	for (int i{}; i < MAX_SOUND_CHANNELS; ++i)
	{
		if (m_currentSounds[i].getStatus() == sf::Sound::Playing)
			return true;
	}
	return false;
}

bool SFMLSoundProvider::isSongPlaying()
{
	if (m_currentSongName != "")
	{
		return m_soundFileCache.getSong(m_currentSongName)->getStatus() == sf::Music::Playing;
	}
	return false;
}