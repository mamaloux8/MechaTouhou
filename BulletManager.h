#ifndef BULLET_MANAGER_H
#define BULLET_MANAGER_H

#include "PlayerBullet.h"
#include "Ennemy.h"
#include "Player.h"
#include "GameObjectManager.h"


class BulletManager
	/*: public Observer<Ennemy>*/
{

public:
	BulletManager();
	~BulletManager();

	/*virtual void onNotify(int lives);*/

	void add(Bullet*);

	void drawAll(sf::RenderWindow& renderWindow);

	void updateAll();
	void clear() { m_clear = true; };

	int getCount() { return m_bullets.size(); };

	void searchEnnemies() { m_updateEnnemies = true; };

	void restartDelta() { m_clock.restart(); };
	void pauseDelta() { m_pauseTime = m_clock.restart().asSeconds(); };

private:

	std::vector<Bullet*> m_bullets{};
	
	sf::Clock m_clock;
	float m_pauseTime{ 0 };

	Player* m_player{};

	//std::vector<std::string> m_ennemies{};
	std::vector<std::pair<std::string, Ennemy*>> m_ennemies{};
	bool m_updateEnnemies{true};
	bool m_clear{ false };

	struct BulletDeallocator
	{
		void operator()(const Bullet* e) const
		{
			delete e;
		}

	};

	void wavyShot(Bullet*, float);
	void curvedShot(Bullet*, float);

	bool checkPlayerCollision(Bullet*);
	bool checkEnnemyCollision(Bullet*, GameObjectManager&);
	bool checkOutOfBounds(Bullet*);
	void deleteBullet(Bullet*, size_t);
	void clearBullets();

	sf::Clock m_ticks{};

};

#endif
