#include <cassert>
#include "EnergyBall.h"
#include "Game.h"


EnergyBall::EnergyBall(MathVector velocity)
	: m_velocity{velocity}
{
	load("res/img/EnergyBallSpritesheet.png");
	assert(isLoaded());

	getSprite().setTextureRect(m_animRects[0]);
	getSprite().setOrigin(getSprite().getLocalBounds().width / 2, getSprite().getLocalBounds().height / 2);
	getSprite().setScale(2.f, 2.f);
}


void EnergyBall::draw(sf::RenderWindow& rw)
{
	VisibleGameObject::draw(rw);
}

void EnergyBall::update(float elapsedTime)
{
	getSprite().move(m_velocity.getX() * elapsedTime, m_velocity.getY() * elapsedTime);

	if (m_animClock.getElapsedTime().asSeconds() >= 0.05f)
	{
		++m_explosionAnimStep;

		if (m_explosionAnimStep >= EXPLOSION_ANIM_NB_STEPS)
			m_explosionAnimStep = 0;

		getSprite().setTextureRect(m_animRects[m_explosionAnimStep]);
		m_animClock.restart();
	}

	if (checkOutOfBounds())
		destroy();
}

bool EnergyBall::checkOutOfBounds()
{
	if (getPosition().x < getWidth() / 2 - 500.f
		|| getPosition().x > Game::SCREEN_WIDTH - getWidth() / 2 + 500.f
		|| getPosition().y < getHeight() / 2 - 500.f
		|| getPosition().y > Game::SCREEN_HEIGHT - getHeight() / 2 + 500.f)
	{
		return true;
	}

	return false;
}