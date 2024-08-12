#include "Hitbox.h"
#include <cassert>

Hitbox::Hitbox()
{
	load("res/img/Hitbox.png");
	assert(isLoaded());

	getSprite().setScale(0.8f, 0.8f);
	getSprite().setOrigin(getSprite().getLocalBounds().width / 2, getSprite().getLocalBounds().height / 2);
}

Hitbox::~Hitbox()
{
}

void Hitbox::draw(sf::RenderWindow& rw)
{
	VisibleGameObject::draw(rw);
}