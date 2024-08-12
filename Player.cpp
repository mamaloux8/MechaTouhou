#include <cassert>

#include "Game.h"
#include "Player.h"
#include "Utilities.h"
#include "ServiceLocator.h"
#include "EnergyBall.h"

Player::Player()
	: m_lives{8}
	, m_normalSpeed{1000.f}
	, m_slowSpeed{350.f}
	, m_fastSpeed{1500.f}
	, m_speed{m_normalSpeed}
	, m_normalShot{270, 2000.0}
	, m_invincibleTime{sf::seconds(3.f)}
	, m_flickeringTime{sf::milliseconds(50.f)}
	, m_lManTexture{ Game::getTextureCache().getImage("res/img/Lman.png") }
	, m_mManTexture{ Game::getTextureCache().getImage("res/img/Mman.png") }
{
	load("res/img/Kman.png");
	assert(isLoaded());

	getSprite().setOrigin(getSprite().getLocalBounds().width / 2, getSprite().getLocalBounds().height / 2);
}

Player::~Player()
{
}

void Player::draw(sf::RenderWindow& rw)
{
	VisibleGameObject::draw(rw);
	m_hitbox.draw(rw);
	m_flash.draw(rw);
}

void Player::update(float elapsedTime)
{
	invincible();
	m_flash.update(elapsedTime);

	if (!m_isPlayable)
		return;

	//reduce the knockback velocity to make it smooth
	//put in a function
	
	if (m_knockbackVelocity.x > -1.f && m_knockbackVelocity.x < 1.f)
		m_knockbackVelocity.x = 0.f;
	else if (m_knockbackVelocity.x > 1.f)
		m_knockbackVelocity.x -= 2000.f * elapsedTime;
	else if (m_knockbackVelocity.x < -1.f)
		m_knockbackVelocity.x += 2000.f * elapsedTime;

	if (m_knockbackVelocity.y > -1.f && m_knockbackVelocity.y < 1.f)
		m_knockbackVelocity.y = 0.f;
	else if (m_knockbackVelocity.y > 1.0f)
		m_knockbackVelocity.y -= 2000.f * elapsedTime;
	else if (m_knockbackVelocity.y < -1.0f)
		m_knockbackVelocity.y += 2000.f * elapsedTime;

	if (m_pushTimer.getElapsedTime().asSeconds() > 0.2f)
		m_isPushed = false;



	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Right) && !m_isPushed)
	{
		m_velocity.x = 1;
	}
	else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Left) && !m_isPushed)
	{
		m_velocity.x = -1;
	}
	else
	{
		m_velocity.x = 0;
	}
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Up) && !m_isPushed)
	{
		m_velocity.y = -1;
	}
	else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Down) && !m_isPushed)
	{
		m_velocity.y = 1;
	}
	else
	{
		m_velocity.y = 0;
	}

	//rajouter les tirs max sur écran
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Y) && m_shotCooldown.getElapsedTime() > sf::milliseconds(100))
	{
		//Faire un truc qui vise ?
		//RAJOUTER NOMBRE MAX PROJECTILE A L'ECRAN POUR AUGMENTER EFFICACITE DE QUAND ON EST PROCHES
		if (m_playerMode != normal)
		{
			if(m_changeTimer.getElapsedTime().asSeconds() > 0.2f)
				changeMode(normal);
			return;
		}

		m_shotCooldown.restart();
	
		m_normalShot.setPosition(getPosition().x-25, getPosition().y-50);
		Game::getBulletManager().add(new PlayerBullet{m_normalShot});

		m_normalShot.setPosition(getPosition().x+25, getPosition().y-50);
		Game::getBulletManager().add(new PlayerBullet{ m_normalShot});

		m_normalShot.setPosition(getPosition().x + 50, getPosition().y - 50);
		m_normalShot.setAngle(277);
		Game::getBulletManager().add(new PlayerBullet{ m_normalShot });

		m_normalShot.setPosition(getPosition().x - 50, getPosition().y - 50);
		m_normalShot.setAngle(263);
		Game::getBulletManager().add(new PlayerBullet{ m_normalShot });

		m_normalShot.setAngle(270);

		if (!ServiceLocator::getAudio()->isSoundPlaying())
		{
			ServiceLocator::getAudio()->playSound("res/se/shoot2.ogg");
		}
	}

	if (sf::Keyboard::isKeyPressed(sf::Keyboard::X) && m_shotCooldown.getElapsedTime() > sf::milliseconds(75))
	{
		//Pour le mode fast mettre des lasers, pour se débarasser facilement des petits ennemis
		if (m_playerMode != fast)
		{
			if (m_changeTimer.getElapsedTime().asSeconds() > 0.2f)
				changeMode(fast);
			return;
		}

		m_shotCooldown.restart();

		for (int i{}; i < 8; ++i)
		{
			m_normalShot.setPosition(getPosition().x - 80+20*i, getPosition().y - 50);
			m_normalShot.setAngle(210+15*i);
			Game::getBulletManager().add(new PlayerBullet{ m_normalShot });
		}

		m_normalShot.setAngle(270);

		if (!ServiceLocator::getAudio()->isSoundPlaying())
		{
			ServiceLocator::getAudio()->playSound("res/se/shoot2.ogg");
		}
	}

	if (sf::Keyboard::isKeyPressed(sf::Keyboard::C) && m_shotCooldown.getElapsedTime() > sf::milliseconds(100))
	{
		//Faire sprite missiles
		if (m_playerMode != slow)
		{
			if (m_changeTimer.getElapsedTime().asSeconds() > 0.2f)
				changeMode(slow);
			return;
		}

		m_shotCooldown.restart();

		m_normalShot.setPosition(getPosition().x - 15, getPosition().y - 50);
		Game::getBulletManager().add(new PlayerBullet{ m_normalShot });

		m_normalShot.setPosition(getPosition().x + 15, getPosition().y - 50);
		Game::getBulletManager().add(new PlayerBullet{ m_normalShot });

		m_normalShot.setPosition(getPosition().x + 35, getPosition().y - 50);
		Game::getBulletManager().add(new PlayerBullet{ m_normalShot });

		m_normalShot.setPosition(getPosition().x - 35, getPosition().y - 50);
		Game::getBulletManager().add(new PlayerBullet{ m_normalShot });

		if (!ServiceLocator::getAudio()->isSoundPlaying())
		{
			ServiceLocator::getAudio()->playSound("res/se/shoot2.ogg");
		}
	}

	if (getPosition().x < getSprite().getLocalBounds().width / 2)
	{
		setPosition(getSprite().getLocalBounds().width/2, getPosition().y);
	}
	if (getPosition().x > (Game::SCREEN_WIDTH - getSprite().getLocalBounds().width / 2))
	{
		setPosition(Game::SCREEN_WIDTH - getSprite().getLocalBounds().width / 2, getPosition().y);
	}
	if (getPosition().y < getSprite().getLocalBounds().height/ 2)
	{
		setPosition(getPosition().x, getSprite().getLocalBounds().height / 2);
	}
	if (getPosition().y > (Game::SCREEN_HEIGHT - getSprite().getLocalBounds().height / 2))
	{
		setPosition(getPosition().x, Game::SCREEN_HEIGHT- getSprite().getLocalBounds().height / 2);
	}





	sf::Vector2f velocity{ (m_velocity.x * m_speed + m_knockbackVelocity.x)*elapsedTime, (m_velocity.y * m_speed + m_knockbackVelocity.y)*elapsedTime};

	if (m_velocity.x != 0 && m_velocity.y != 0)
	{
		float vectorLength{ getVectorLength(m_velocity) };
		velocity.x /= vectorLength;
		velocity.y /= vectorLength;
	}

	getSprite().move(velocity.x, velocity.y);
	m_hitbox.setPosition(getPosition().x-2, getPosition().y+5); 

	//Set Position of flash animation
	m_flash.setPosition(getPosition().x, getPosition().y);

}

void Player::getHit()
{
	ServiceLocator::getAudio()->playSound("res/se/DeathK.wav");
	spawnEnergyBalls(10);

	if (m_lives > 0)
	{
		startInvincibility();
		m_lives--;
		setPosition(Game::SCREEN_WIDTH/2, Game::SCREEN_HEIGHT-250);
	}
	else
	{
		Game::setGameState(Game::exiting);
	}

	m_playerSubject.notify(this, Observer<Player>::isHit);
	
}

void Player::spawnEnergyBalls(int nbBalls)
{
	EnergyBall ball{ {0.f,1000.f} };
	ball.setPosition(getPosition().x, getPosition().y);

	for (int i{}; i < nbBalls; ++i)
	{
		ball.setAngle((360.f / static_cast<float>(nbBalls)) * i);
		EnergyBall* energyBall{ new EnergyBall{ball} };
		Game::getGameObjectManager().add(ball.getName(), energyBall);
	}
}

void Player::startInvincibility()
{
	m_isInvincible = true;
	m_invincibleTimer.restart();
	m_flickeringTimer.restart();
}

void Player::invincible()
{
	if (m_invincibleTimer.getElapsedTime() > m_invincibleTime || !m_isInvincible)
	{	
		m_isInvincible = false;
		getSprite().setColor(sf::Color{ 255,255,255,255 });
		return;
	}

	if (m_flickeringTimer.getElapsedTime() > m_flickeringTime)
	{
		if (m_isVisible)
		{
			getSprite().setColor(sf::Color{ 0,0,0,0 });
		}
		else
		{
			getSprite().setColor(sf::Color{ 255,255,255,255 });
		}
		m_flickeringTimer.restart();
		m_isVisible = !m_isVisible;
	}
		
}

void Player::changeMode(PlayerMode playerMode)
{
	ServiceLocator::getAudio()->playSound("res/se/Change.wav");
	m_flash.play();

	switch (playerMode)
	{
	case normal:
		m_playerMode = normal;
		m_speed = m_normalSpeed;
		getSprite().setTexture(*getTexture(), true);
		break;

	case fast:
		m_playerMode = fast;
		m_speed = m_fastSpeed;
		getSprite().setTexture(*m_lManTexture, true);
		break;

	case slow:
		m_playerMode = slow;
		m_speed = m_slowSpeed;
		getSprite().setTexture(*m_mManTexture, true);
		break;
	}

	m_changeTimer.restart();
}

void Player::knockback(float posX, float posY)
{
	if (getPosition().x > posX)
		m_knockbackVelocity.x = 800.f;
	if (getPosition().x < posX)
		m_knockbackVelocity.x = -800.f;

	if (getPosition().y > posY)
		m_knockbackVelocity.y = 800.f;
	if (getPosition().y < posY)
		m_knockbackVelocity.y = -800.f;

	m_pushTimer.restart();
	m_isPushed = true;
}

void Player::onNotify(DialogueObserver::State state)
{
	if (state == DialogueObserver::hasBegun)
		m_isPlayable = false;
	else if (state == DialogueObserver::hasEnded)
		m_isPlayable = true;

}