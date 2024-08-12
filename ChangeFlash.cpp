#include <cassert>
#include "ChangeFlash.h"
#include "Game.h"



ChangeFlash::ChangeFlash()
{
	load("res/img/ChangeSpritesheet.png");
	assert(isLoaded());

	getSprite().setTextureRect(m_flashRects[0]);
	getSprite().setOrigin(getSprite().getLocalBounds().width / 2, getSprite().getLocalBounds().height / 2);
}

void ChangeFlash::draw(sf::RenderWindow& rw)
{
	VisibleGameObject::draw(rw);
}

void ChangeFlash::update(float elapsedTime)
{
	if (!m_play)
	{
		getSprite().setColor(sf::Color{ 255,255,255,0 });
		return;
	}
	getSprite().setColor(sf::Color{ 255,255, 255,255 });

	if (m_animClock.getElapsedTime().asSeconds() >= 0.05f)
	{
		++m_animStep;
		if (m_animStep >= ANIM_NB_STEPS)
		{
			m_animStep = 0;
			m_play = false;
		}
		getSprite().setTextureRect(m_flashRects[m_animStep]);
		m_animClock.restart();
	}
}