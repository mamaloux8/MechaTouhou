#ifndef PLAYER_BULLET_H	
#define PLAYER_BULLET_H

#include "Bullet.h"

class PlayerBullet
	: public Bullet
{

public:
	PlayerBullet();
	PlayerBullet(int angle, float velocity);
	~PlayerBullet();

private:
	void instantiate();
	

};

#endif

