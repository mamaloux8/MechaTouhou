#include "BulletPattern.h"
#include "Random.h"
#include "Game.h"


#include <typeinfo>
#include "BulletTypes.h"

BulletPattern::~BulletPattern()
{
	delete m_type2;
}

BulletPattern::BulletPattern(Bullet* bullet, AngleReference angleReference, float angleOffset)
	: m_bullet{ bullet }
	, m_type{ singleShot }
	, m_bulletBaseVelocity{ m_bullet->getVelocityVector() }
	, m_angleReference{angleReference}
	, m_angleOffset{angleOffset}
{
	m_player = Game::getGameObjectManager().get("Player");
}

BulletPattern::BulletPattern(Bullet* bullet)
	: BulletPattern(bullet, ignoreReference)
{
}

BulletPattern::BulletPattern(Bullet* bullet, Type2* type)
	: m_bullet{ bullet }
	, m_type{ type->getType() }
	, m_type2{type}
	, m_bulletBaseVelocity{ m_bullet->getVelocityVector() }
{
	m_player = Game::getGameObjectManager().get("Player");
}

BulletPattern::BulletPattern(Bullet* bullet, float rotation)
	: m_bullet{ bullet }
	, m_type{singleShot}
	, m_rotation{rotation}
	, m_bulletBaseVelocity{ m_bullet->getVelocityVector() }
{
	m_player = Game::getGameObjectManager().get("Player");
}

void BulletPattern::spawnCircle(int nbBullets)
{
	setBulletAngle();
	for (int i{}; i < nbBullets; ++i)
	{
		m_bullet->setAngle((360.f / static_cast<float>(nbBullets)) * i + m_bullet->getBaseAngle() + Random::get(-5,5));
		Bullet* bullet{ new Bullet{*m_bullet} };
		Game::getBulletManager().add(bullet);
	}
}

void BulletPattern::spawnSingleShot()
{
	setBulletAngle();
	Bullet* bullet{ new Bullet{*m_bullet} };
	Game::getBulletManager().add(bullet);
}

void BulletPattern::setBulletPosition(sf::Vector2f position)
{
	m_bullet->setPosition(position.x, position.y);
}

void BulletPattern::setBulletAngle()
{
	if (m_bullet->getAngleType() == Bullet::random)
	{
		m_bullet->setAngle(Random::get(0, 360));
	}
	else if (m_bullet->getAngleType() == Bullet::aimed)
	{
		m_bullet->setAngle(m_bullet->getAngleBetween(*m_player));
	}
	else if (m_bullet->getAngleType() == Bullet::fixed)
	{
		m_bullet->setAngle(m_bullet->getAngleReference()->getAngle());
	}
	m_bullet->setBaseAngle(m_bullet->getAngle());
}

void BulletPattern::rotate(float elapsedTime)
{
	m_bullet->setAngle(m_bullet->getAngle() + m_rotation * elapsedTime);
}