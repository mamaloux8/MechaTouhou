#include <assert.h>
#include "PlayerBullet.h"
#include "Game.h"

PlayerBullet::PlayerBullet()
	:PlayerBullet(0,0)
{
}

PlayerBullet::PlayerBullet(int angle, float length)
{
	m_velocity = MathVector{ angle, length };
	instantiate();
}

PlayerBullet::~PlayerBullet()
{
}

void PlayerBullet::instantiate()
{
	load("res/img/PlayerBullet.png");
	assert(isLoaded());

	setPosition(Game::SCREEN_WIDTH / 2, Game::SCREEN_HEIGHT / 2);
	getSprite().setOrigin(getSprite().getLocalBounds().width / 2, getSprite().getLocalBounds().height / 2);
	getSprite().setScale(sf::Vector2f{ 2.f,2.f });
}