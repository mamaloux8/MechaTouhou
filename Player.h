#ifndef PLAYER_H
#define PLAYER_H

#include "VisibleGameObject.h"
#include "PlayerBullet.h"
#include "Hitbox.h"

#include "ServiceLocator.h"
#include "Subject.h"
#include "DialogueSubject.h"
#include "ChangeFlash.h"

class Player :
	public VisibleGameObject, public DialogueObserver
{

public:
	Player();
	~Player();

	void update(float elapsedTime);
	void draw(sf::RenderWindow& rw);

	void startInvincibility();
	void invincible();

	const Hitbox& getHitbox() const { return m_hitbox; };
	const bool getIsInvincible() const { return m_isInvincible; };

	void getHit();
	void knockback(float posX, float posY);

	void spawnEnergyBalls(int);

	int getLives() const { return m_lives; };

	Subject<Player>& getPlayerSubject() { return m_playerSubject; };

	virtual void onNotify(DialogueObserver::State);

protected:
	enum PlayerMode
	{
		normal,
		fast,
		slow,
	};

	void changeMode(PlayerMode);

private:

	bool m_isPlayable{ true };

	int m_lives{};
	PlayerMode m_playerMode{normal};
	ChangeFlash m_flash{};
	sf::Clock m_changeTimer{};

	float m_normalSpeed{};
	float m_fastSpeed{};
	float m_slowSpeed{};

	float m_speed{};

	sf::Vector2<int> m_velocity;
	sf::Vector2<float> m_knockbackVelocity;
	bool m_isPushed{ false };
	sf::Clock m_pushTimer{};

	Hitbox m_hitbox{};

	sf::Clock m_shotCooldown{};
	PlayerBullet m_normalShot{};

	bool m_isInvincible{false};
	sf::Time m_invincibleTime{};
	sf::Clock m_invincibleTimer{};

	bool m_isVisible{ true };
	sf::Time m_flickeringTime{};
	sf::Clock m_flickeringTimer{};

	sf::Texture* m_lManTexture{};
	sf::Texture* m_mManTexture{};

	//Puisque la classe du Subject est player, peut-être ce serait mieux de faire que Player est dérivé de Subject ???
	//https://www.codeproject.com/Articles/3267/Implementing-a-Subject-Observer-Pattern-with-Templ
	Subject<Player> m_playerSubject{};

};

#endif

