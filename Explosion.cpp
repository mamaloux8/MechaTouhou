#include <cassert>
#include "Game.h"
#include "Explosion.h"


Explosion::Explosion()
{
	load("res/img/ExplosionSpritesheet.png");
	assert(isLoaded());

	getSprite().setTextureRect(m_explosionRects[0]);
	getSprite().setOrigin(getSprite().getLocalBounds().width / 2, getSprite().getLocalBounds().height / 2);
}

void Explosion::draw(sf::RenderWindow& rw)
{
	VisibleGameObject::draw(rw);
}

void Explosion::update(float elapsedTime)
{
	if (m_animClock.getElapsedTime().asSeconds() >= 0.05f)
	{
		++m_explosionAnimStep;
		if (m_explosionAnimStep >= EXPLOSION_ANIM_NB_STEPS)
		{
			destroy();
			return;
		}
		
		getSprite().setTextureRect(m_explosionRects[m_explosionAnimStep]);
		m_animClock.restart();
	}
}