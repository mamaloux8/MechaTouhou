#ifndef ENNEMY_H
#define ENNEMY_H

#include <vector>
#include <array>
#include "VisibleGameObject.h"
#include "BulletPattern.h"
#include "Subject.h"
#include "Player.h"

class Ennemy
	: public VisibleGameObject
{
public:
	Ennemy(std::vector<std::pair<BulletPattern*, sf::Time>>, int);
	Ennemy(std::vector<std::pair<BulletPattern*, sf::Time>>, int, int, std::pair<std::string, std::vector<sf::IntRect>>*);
	Ennemy(std::vector<std::pair<BulletPattern*, sf::Time>>, int, int, std::pair<std::string, std::vector<sf::IntRect>>*, float scale);

	~Ennemy();

	void update(float elapsedTime);
	void draw(sf::RenderWindow& rw);

	virtual void getOutOfBounds();

	virtual void getHit();
	virtual void die();

	int getHealth() const { return m_health; };
	int getBaseHealth() const { return m_baseHealth; };
	int getScore() const { return m_score; };

	void setVelocity(MathVector& velocity) { m_velocity = velocity; };
	void setAcceleration(MathVector& acceleration) { m_acceleration = acceleration; };

	static Subject<Ennemy>& getEnnemySubject() { return m_ennemySubject; }
	void resetTimers() { m_clock.restart(); m_moveTimer.restart(); };

	void updateMoveTimeline();

	static void setPlayer(Player* player) { m_player = player; };
	
	void setMoveTimeline(std::vector<std::pair<std::pair<MathVector, MathVector>, sf::Time>> timeline) { m_moveTimeline = timeline; };

	static sf::Shader m_whiteShader;

protected:
	//Comme player, utiliser l'héritage
	static Subject<Ennemy> m_ennemySubject;

	int m_baseHealth{};
	int m_health{};
	int m_score{};

	std::vector<std::pair<std::pair<MathVector, MathVector>, sf::Time>> m_moveTimeline{};
	std::vector<std::pair<BulletPattern*, sf::Time>> m_patternQueue{};

private:
	MathVector m_velocity{};
	MathVector m_acceleration{};

	sf::Clock m_moveTimer{};

	const std::pair<std::string, std::vector<sf::IntRect>>* m_animator{ nullptr };
	int animStep{0};
	sf::Clock m_animClock{};

	

	sf::Clock m_clock{};
	int m_patternRepeatCounter{};

	sf::Clock m_flashTimer{};

	//Useful when some bullets must have the same angle as a aimed bullet
	float m_referenceAngle{};

	static Player* m_player;
};

#endif
