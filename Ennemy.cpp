#include <cassert>
#include "Game.h"
#include "Ennemy.h"
#include "Explosion.h"

Ennemy::Ennemy(std::vector<std::pair<BulletPattern*, sf::Time>> queue, int health)
	: m_patternQueue{queue}
	, m_health{health}
	, m_baseHealth{health}
{
	load("res/img/Ennemy.png");
	assert(isLoaded());

	getSprite().setOrigin(getSprite().getLocalBounds().width / 2, getSprite().getLocalBounds().height / 2);
}

Ennemy::Ennemy(std::vector<std::pair<BulletPattern*, sf::Time>> queue, int health, int score, std::pair<std::string, std::vector<sf::IntRect>>* animator, float scale)
	: m_patternQueue{ queue }
	, m_health{ health }
	, m_baseHealth{ health }
	, m_animator { animator }
	, m_score { score }
{
	load(m_animator->first);
	assert(isLoaded());

	getSprite().setTextureRect(m_animator->second[0]);
	getSprite().setOrigin(getSprite().getLocalBounds().width / 2, getSprite().getLocalBounds().height / 2);
	getSprite().setScale(sf::Vector2f{ scale, scale });
}

Ennemy::Ennemy(std::vector<std::pair<BulletPattern*, sf::Time>> queue, int health, int score, std::pair<std::string, std::vector<sf::IntRect>>* animator)
	: Ennemy(queue, health, score, animator, 1.f)
{
}

Ennemy::~Ennemy()
{
}

void Ennemy::draw(sf::RenderWindow& rw)
{

	if (m_flashTimer.getElapsedTime() <= sf::milliseconds(20))
	{
		//from left to right: red,green,blue,alpha. Max is 1, not 255
		m_whiteShader.setUniform("flashColor", sf::Glsl::Vec4(1, 1, 1, 1));
		VisibleGameObject::draw(rw, m_whiteShader);
	}
	else
	{
		VisibleGameObject::draw(rw);
	}

}

void Ennemy::update(float elapsedTime)
{
	if (m_animator != nullptr)
	{
		if (m_animClock.getElapsedTime().asSeconds() >= 0.1f)
		{
			getSprite().setTextureRect(m_animator->second[animStep]);
			++animStep;
			m_animClock.restart();
			if (animStep >= m_animator->second.size())
				animStep = 0;
		}
	}

	updateMoveTimeline();

	getOutOfBounds();

	//See if it touches the player
	if (getBoundingRect().intersects(m_player->getHitbox().getBoundingRect()))
	{
		m_player->knockback(getPosition().x, getPosition().y);
	}

	getSprite().move(m_velocity.getX() * elapsedTime, m_velocity.getY() * elapsedTime);
	m_velocity = { m_velocity.getX() + m_acceleration.getX() * elapsedTime,  m_velocity.getY()  + m_acceleration.getY() * elapsedTime};

	if (m_patternQueue.size() <= 0 || getPosition().y > Game::SCREEN_HEIGHT-100 || getPosition().y < 0)
	{
		return;
	}

	auto& pattern{ m_patternQueue.back() };
	if (m_clock.getElapsedTime() >= pattern.second)
	{
		m_clock.restart();
		pattern.first->setBulletPosition(sf::Vector2f{ getPosition().x, getPosition().y });
		pattern.first->rotate(elapsedTime);

		//Check angle reference

		if (pattern.first->getAngleReference() == BulletPattern::useReference)
			pattern.first->setBulletAngle(m_referenceAngle + pattern.first->getAngleOffset());

		switch (pattern.first->getType())
		{
		case BulletPattern::circle:
			pattern.first->spawnCircle(pattern.first->getType2()->getNbBullets());
			break;

		case BulletPattern::singleShot:
			pattern.first->spawnSingleShot();
		}

		//Check angle reference
		if (pattern.first->getAngleReference() == BulletPattern::fixReference)
			m_referenceAngle = pattern.first->getAngle();

		if (pattern.first->getRepeatCounter() > 0 && m_patternRepeatCounter < 1)
		{
			auto test{ pattern.first->getRepeatCounter() };

			m_patternRepeatCounter = pattern.first->getRepeatCounter();
			pattern.first->setSpeed(pattern.first->getSpeed() + pattern.first->getAcceleration() * elapsedTime );
		}
		else if(m_patternRepeatCounter > 0)
		{
			pattern.first->setSpeed(pattern.first->getSpeed() + pattern.first->getAcceleration() * elapsedTime);
			--m_patternRepeatCounter;
		}

		if (m_patternRepeatCounter < 1)
		{
			if (pattern.first->getAcceleration() != 0)
			{
				pattern.first->setSpeed(pattern.first->getBaseVelocity().getLength());
				//à enlever ?
				//pattern.first->setBulletAngle(pattern.first->getBaseVelocity().getAngle());
			}
			m_patternQueue.insert(m_patternQueue.begin(), pattern);
			m_patternQueue.pop_back();
		}
	}
}

void Ennemy::getHit()
{
	--m_health;
	m_flashTimer.restart();
}

void Ennemy::die()
{
	ServiceLocator::getAudio()->playSound("res/se/Explosion.wav");

	Explosion explosion{};
	explosion.setPosition(getPosition().x, getPosition().y);
	Game::getGameObjectManager().add(explosion.getName(), new Explosion{explosion});

	m_ennemySubject.notify(this, Observer<Ennemy>::isDead);
}

void Ennemy::updateMoveTimeline()
{
	if (m_moveTimeline.empty())
		return;

	if (m_moveTimer.getElapsedTime().asSeconds() > m_moveTimeline.back().second.asSeconds())
	{
		m_moveTimeline.pop_back();
		
		if (m_moveTimeline.empty())
			return;

		std::pair<MathVector, MathVector> moveData{ m_moveTimeline.back().first };
		m_velocity = moveData.first;
		m_acceleration = moveData.second;

		m_moveTimer.restart();
	}
}

void Ennemy::getOutOfBounds()
{
	if (getPosition().x < getSprite().getLocalBounds().width / 2 - 500.f)
	{
		destroy();
	}
	if (getPosition().x > (Game::SCREEN_WIDTH - getSprite().getLocalBounds().width / 2) + 500.f)
	{
		destroy();
	}
	if (getPosition().y < getSprite().getLocalBounds().height / 2 - 500.f)
	{
		destroy();
	}
	if (getPosition().y > (Game::SCREEN_HEIGHT - getSprite().getLocalBounds().height / 2) + 500.f)
	{
		destroy();
	}
}

Subject<Ennemy> Ennemy::m_ennemySubject{};