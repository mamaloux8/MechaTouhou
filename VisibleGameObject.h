#ifndef VISIBLE_GAMEOBJECT_H
#define VISIBLE_GAMEOBJECT_H

#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>

class VisibleGameObject
{
public:
	VisibleGameObject();
	VisibleGameObject(const VisibleGameObject&) = default;
	virtual ~VisibleGameObject();

	virtual void load(std::string filename);
	virtual void draw(sf::RenderWindow& window);
	virtual void draw(sf::RenderWindow& window, sf::Shader&);
	virtual void update(float elapsedTime);

	virtual void setPosition(float x, float y);
	virtual sf::Vector2f getPosition() const;

	virtual float getWidth() const { return m_sprite.getLocalBounds().width; };
	virtual float getHeight() const { return m_sprite.getLocalBounds().height; };

	virtual float getAngleBetween(VisibleGameObject&);

	virtual sf::Rect<float> getBoundingRect() const { return m_sprite.getGlobalBounds(); };

	virtual bool isLoaded() const { return m_isLoaded; };

	virtual void setName(std::string str);

	virtual bool getValid() const { return m_valid; };
	virtual void destroy() { m_valid = false; };

protected:
	sf::Sprite& getSprite() { return m_sprite; };
	sf::Texture* getTexture() { return m_image; };

private:
	bool m_valid{ true };

	sf::Sprite m_sprite;
	sf::Texture* m_image;
	std::string m_filename;
	bool m_isLoaded;

};

#endif