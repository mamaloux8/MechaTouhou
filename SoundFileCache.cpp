#include "SoundFileCache.h"

SoundFileCache::SoundFileCache()
{
}

SoundFileCache::~SoundFileCache()
{
	std::for_each(m_sounds.begin(), m_sounds.end(), Deallocator<sf::SoundBuffer*>());
	std::for_each(m_music.begin(), m_music.end(), Deallocator<sf::Music*>());
}

sf::Sound SoundFileCache::getSound(std::string soundName) const
{
	std::map<std::string, sf::SoundBuffer*>::iterator itr = m_sounds.find(soundName);
	if (itr == m_sounds.end())
	{
		sf::SoundBuffer* soundBuffer = new sf::SoundBuffer();
		if (!soundBuffer->loadFromFile(soundName))
		{
			delete soundBuffer;
			throw SoundNotFoundException(soundName + " was not found in call to SoundFileCache::getSound");
		}

		std::map<std::string, sf::SoundBuffer*>::iterator res =
			m_sounds.insert(std::pair<std::string, sf::SoundBuffer*>(soundName, soundBuffer)).first;

		sf::Sound sound;
		sound.setBuffer(*soundBuffer);
		return sound;

	}
	else
	{
		sf::Sound sound;
		sound.setBuffer(*itr->second);
		return sound;
	}

	throw SoundNotFoundException(
		soundName + " was not found in call to SoundFileCache::getSound");
}

sf::Music* SoundFileCache::getSong(std::string soundName) const
{
	std::map<std::string, sf::Music*>::iterator itr = m_music.find(soundName);
	if (itr == m_music.end())
	{
		sf::Music* song = new sf::Music();
		if (!song->openFromFile(soundName))
		{
			delete song;
			throw SoundNotFoundException(soundName + " was not found in call to SoundFileCache::getSong");
		}

		std::map<std::string, sf::Music*>::iterator res =
			m_music.insert(std::pair<std::string, sf::Music*>(soundName, song)).first;
		return res->second;

	}
	else
	{
		return itr->second;
	}

	throw SoundNotFoundException(
		soundName + " was not found in call to SoundFileCache::getSong");
}

std::map<std::string, sf::SoundBuffer*> SoundFileCache::m_sounds;
std::map<std::string, sf::Music*> SoundFileCache::m_music;