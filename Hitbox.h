#ifndef HITBOX_H
#define HITBOX_H

#include "VisibleGameObject.h"

class Hitbox
	: public VisibleGameObject
{
public:
	Hitbox();
	~Hitbox();

	void draw(sf::RenderWindow& rw);

};

#endif