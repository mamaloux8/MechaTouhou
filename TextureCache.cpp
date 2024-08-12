#include "TextureCache.h"

TextureCache::TextureCache()
{
}

TextureCache::~TextureCache()
{
	std::for_each(m_images.begin(), m_images.end(), TextureDeallocator());
}

sf::Texture* TextureCache::getImage(std::string filename)
{
	std::map<std::string, sf::Texture*>::iterator itr = m_images.find(filename);

	if (itr == m_images.end())
	{
		sf::Texture* image = new sf::Texture();
		if (!image->loadFromFile(filename))
		{
			delete image;
			throw ImageNotFoundException(filename + " was not found in call to TextureCache::getImage");
		}

		std::map<std::string, sf::Texture*>::iterator res =
			m_images.insert(std::pair<std::string, sf::Texture*>(filename, image)).first;
		return image;

	}
	else
	{
		return itr->second;
	}

	throw ImageNotFoundException(
		filename + " was not found in call to TextureCache::getImage");
}

//std::map<std::string, sf::Texture*> TextureCache::m_images;