#ifndef SOUNDFILE_CACHE
#define SOUNDFILE_CACHE

#include "SFML/Audio.hpp"
#include <SFML/Graphics.hpp>

class SoundFileCache
{
public:
	SoundFileCache();
	~SoundFileCache();

	sf::Sound getSound(std::string) const;
	sf::Music* getSong(std::string) const;

private:
	static std::map<std::string, sf::SoundBuffer*> m_sounds;
	static std::map<std::string, sf::Music*> m_music;

	template <typename T>
	struct Deallocator {
		void operator()(const std::pair<std::string, T>& p)
		{
			delete p.second;
		}
	};
};

class SoundNotFoundException : public std::runtime_error
{
public:
	SoundNotFoundException(std::string const& msg) :
		std::runtime_error{ msg }
	{
	}
};

#endif