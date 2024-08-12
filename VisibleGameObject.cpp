#include "VisibleGameObject.h"
#include "Game.h"
#include "Utilities.h"

VisibleGameObject::VisibleGameObject() :
	m_isLoaded{ false }
{
}

VisibleGameObject::~VisibleGameObject()
{
}

void VisibleGameObject::load(std::string filename)
{
	m_image = Game::getTextureCache().getImage(filename);
	m_sprite.setTexture(*m_image);
	m_isLoaded = true;
}

void VisibleGameObject::draw(sf::RenderWindow& renderWindow)
{
	if (m_isLoaded)
	{
		renderWindow.draw(m_sprite);
	}
}

void VisibleGameObject::draw(sf::RenderWindow& renderWindow, sf::Shader& shader)
{
	if (m_isLoaded)
	{
		renderWindow.draw(m_sprite, &shader);
	}
}

void VisibleGameObject::update(float elapsedTime)
{
}

void VisibleGameObject::setPosition(float x, float y)
{
	if (m_isLoaded)
	{
		m_sprite.setPosition(x, y);
	}
}

sf::Vector2f VisibleGameObject::getPosition() const
{
	if (m_isLoaded)
	{
		return m_sprite.getPosition();
	}
	return sf::Vector2f();
}

float VisibleGameObject::getAngleBetween(VisibleGameObject& obj)
{
	float distanceX{ obj.getPosition().x - getPosition().x };
	float distanceY{ getPosition().y - obj.getPosition().y };

	float angle{ -toDegrees(atan(distanceY / distanceX)) };

	if (distanceX < 0)
	{
		angle += 180;
	}

	return angle;
}

void VisibleGameObject::setName(std::string str)
{
}