#ifndef TEXTURE_CACHE_H
#define TEXTURE_CACHE_H

#include <SFML/Graphics.hpp>
#include <string_view>

class TextureCache
{
public:
	TextureCache();
	~TextureCache();

	sf::Texture* getImage(std::string const);

private:
	static inline std::map<std::string, sf::Texture*> m_images;

	struct TextureDeallocator {
		void operator()(const std::pair<std::string, sf::Texture*>& p) const
		{
			delete p.second;
		}
	};
};

class ImageNotFoundException : public std::runtime_error
{
public:
	ImageNotFoundException(std::string const& msg) :
		std::runtime_error{ msg }
	{
	}
};

#endif
