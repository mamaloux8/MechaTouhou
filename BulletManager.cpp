#include "BulletManager.h"
#include "Game.h"
#include <cassert>


BulletManager::BulletManager()
{
}

BulletManager::~BulletManager()
{
	std::for_each(m_bullets.begin(), m_bullets.end(), BulletDeallocator());
}

void BulletManager::add(Bullet* bullet)
{
	m_bullets.push_back(bullet);
}

void BulletManager::drawAll(sf::RenderWindow& rw)
{
	for (auto& e : m_bullets)
	{
		e->draw(rw);
	}
}

void BulletManager::updateAll()
{

	float deltaTime = m_clock.restart().asSeconds();
	deltaTime += m_pauseTime;
	m_pauseTime = 0;

	GameObjectManager& gm{ Game::getGameObjectManager() };

	if (m_bullets.empty())
		return;

	if (m_player == NULL)
	{
		m_player = dynamic_cast<Player*>(Game::getGameObjectManager().get("Player"));
		return;
	}

	if (m_updateEnnemies)
	{
		m_ennemies = Game::getGameObjectManager().getObjectsOfType<Ennemy>();
		m_updateEnnemies = false;
	}

	for (size_t i{}; i < m_bullets.size(); ++i)
	{
		bool destroy{ false };
		Bullet* e{ m_bullets[i] };

		if (e == NULL)
			return;

		switch (e->getBulletType())
		{
		case Bullet::straight_shot:
			break;
		case Bullet::wavy_shot:
			wavyShot(e, deltaTime);
			break;
		case Bullet::curved_shot:
			curvedShot(e, deltaTime);
			break;
		}

		destroy = (checkPlayerCollision(e) || destroy);
		destroy = (checkEnnemyCollision(e, gm) || destroy);
		destroy = (checkOutOfBounds(e) || destroy); 


		if (m_clear)
		{
			clearBullets();
			return;
		}

		if (destroy)
			deleteBullet(e, i);
		else
			e->update(deltaTime);
	}
}

void BulletManager::wavyShot(Bullet* e, float elapsedTime)
{
	e->setAngle(e->getBaseAngle() + sin(e->getTimer().getElapsedTime().asSeconds() * e->getWaveFrequency()) * e->getWaveMagnitude());
}

void BulletManager::curvedShot(Bullet* e, float elapsedTime)
{
	e->setAngle(e->getAngle() + e->getAngleVelocity() * elapsedTime);
}

bool BulletManager::checkPlayerCollision(Bullet* e)
{
	//Pas meilleure manière de faire une hitbox
	sf::FloatRect bulletHitbox{ e->getBoundingRect().left + e->getWidth() / 4.f, e->getBoundingRect().top + e->getHeight() / 4.f, e->getWidth() * 0.75f, e->getHeight() * 0.75f};

	if (bulletHitbox.intersects(m_player->getHitbox().getBoundingRect()) && dynamic_cast<PlayerBullet*>(e) == NULL && !(m_player->getIsInvincible()))
	{
		m_player->getHit();
		return true;
	}

	return false;
}


bool BulletManager::checkEnnemyCollision(Bullet* e, GameObjectManager& gm)
{
	//enlever la condition, elle est juste là pour pas avoir de warning
	if(e == NULL)
		return true;

	if (dynamic_cast<PlayerBullet*>(e) == NULL)
		return false;

	bool destroy{ false };

	for (int i{}; i<m_ennemies.size(); ++i)
	{
		Ennemy* ennemy{ m_ennemies[i].second};
		assert(ennemy != NULL);
			
		if (e->getBoundingRect().intersects(ennemy->getBoundingRect()))
		{
			if (ennemy->getHealth() > 0)
			{
				ennemy->getHit();
			}
			//Dans l'idéal cette fonction devrait que appeller le die d'ennemy, pas le remove lui même 
			//Du coup on se retrouve à un dynamic cast
			else
			{
				Boss* boss = dynamic_cast<Boss*>(ennemy);
				if (boss != NULL && boss->getCurrentState() < boss->getNbStates())
				{
					ennemy->die();
				}
				else
				{
					ennemy->die();
					gm.remove(m_ennemies[i].first);
					m_ennemies.erase(m_ennemies.begin() + i);
					--i;
				}
			}

			destroy = true;
		}
	}

	return destroy;
}

bool BulletManager::checkOutOfBounds(Bullet* e)
{
	if (e->getPosition().x < e->getWidth() / 2 - 500.f
		|| e->getPosition().x > Game::SCREEN_WIDTH - e->getWidth() / 2 + 500.f
		|| e->getPosition().y < e->getHeight() / 2 - 500.f
		|| e->getPosition().y > Game::SCREEN_HEIGHT - e->getHeight() / 2 + 500.f)
	{
		return true;
	}

	return false;
}

void BulletManager::deleteBullet(Bullet* e, size_t index)
{
	delete e;
	e = nullptr;
	m_bullets.erase(m_bullets.begin() + index);
}

void BulletManager::clearBullets()
{

	for (int i{static_cast<int>(m_bullets.size())-1}; i >=0; --i)
	{
		Bullet* b = m_bullets[i];
		delete b;
		b = nullptr;
		m_bullets.pop_back();
	}

	m_clear = false;
}
