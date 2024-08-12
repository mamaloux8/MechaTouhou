#include <cassert>
#include "Game.h"
#include "Bullet.h"
#include "Random.h"

Bullet::Bullet()
	: Bullet(0,0)
{
	//Destroy bullet with observer pattern ??
	//For now I'll just make that it so fast that it gets out of the world quickly
	m_velocity = MathVector{ 90, -100000.f };
}

Bullet::Bullet(const Bullet& b)
{
	*this = b;
	m_timer.restart();
}

Bullet::Bullet(MathVector& velocity)
	: m_velocity{ velocity }
{
	instantiate();
}

Bullet::Bullet(int angle, float length)
{
	m_velocity = MathVector{ angle, length };
	instantiate();
}


Bullet::Bullet(float dx, float dy)
{
	m_velocity = MathVector{ dx, dy };
	instantiate();
}

Bullet::Bullet(float angle, float length, AngleType angleType)
	: m_angleType{ angleType }
{
	m_velocity = MathVector{ static_cast<int>(angle), length};
	instantiate();
}

Bullet::Bullet(float angle, float length, AngleType angleType, std::string textureStr)
	: m_angleType{ angleType }
{
	m_velocity = MathVector{ static_cast<int>(angle), length };
	
	load(textureStr);
	assert(isLoaded());

	setPosition(Game::SCREEN_WIDTH / 2, Game::SCREEN_HEIGHT / 2);
	getSprite().setOrigin(getSprite().getLocalBounds().width / 2, getSprite().getLocalBounds().height / 2);
	getSprite().setScale(sf::Vector2f{ 2.f,2.f });

	m_baseAngle = m_velocity.getAngle();
}

Bullet::Bullet(float angle, float length, Bullet* angleReference)
	: m_angleReference{angleReference}
	, m_angleType{fixed}
	, m_hasReference{true}
{
	m_velocity = MathVector{ static_cast<int>(angle), length };
	instantiate();
}


Bullet::Bullet(float angle, float length, AngleType angleType, float angleVelocity)
	: Bullet(angle, length, angleType)
{
	m_type = curved_shot;
	m_angleVelocity = angleVelocity;
}

Bullet::Bullet(float angle, float length, Bullet* angleReference, float angleVelocity)
	: Bullet(angle, length, angleReference)
{
	m_type = curved_shot;
	m_angleVelocity = angleVelocity;
}


Bullet::Bullet(float angle, float length, AngleType angleType, float magnitude, float frequency)
	: Bullet(angle, length, angleType)
{
	m_type = wavy_shot;
	m_waveMagnitude = magnitude;
	m_waveFrequency = frequency;
}

Bullet::Bullet(float angle, float length, Bullet* angleReference, float magnitude, float frequency)
	: Bullet(angle, length, angleReference)
{
	m_type = wavy_shot;
	m_waveMagnitude = magnitude;
	m_waveFrequency = frequency;
}

void Bullet::instantiate()
{
	load("res/img/Bullet.png");
	assert(isLoaded());

	setPosition(Game::SCREEN_WIDTH / 2, Game::SCREEN_HEIGHT / 2);
	getSprite().setOrigin(getSprite().getLocalBounds().width / 2, getSprite().getLocalBounds().height / 2);
	getSprite().setScale(sf::Vector2f{ 2.f,2.f });

	m_baseAngle = m_velocity.getAngle();
}


Bullet::~Bullet()
{
}

void Bullet::update(float elapsedTime)
{
	getSprite().move(m_velocity.getX() * elapsedTime, m_velocity.getY() * elapsedTime);
}