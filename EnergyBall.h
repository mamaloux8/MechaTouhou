#ifndef ENERGY_BALL
#define ENERGY_BALL

#include <array>
#include "VisibleGameObject.h"
#include "MathVector.h"


class EnergyBall
	: public VisibleGameObject
{
public:
	EnergyBall(MathVector);
	~EnergyBall() = default;

	void update(float elapsedTime);
	void draw(sf::RenderWindow& rw);

	const std::string& getName() const { return m_name; };
	void setName(std::string& str) { m_name = str; };

	void setAngle(float angle) { m_velocity.setAngle(angle); };

	bool checkOutOfBounds();


private:
	MathVector m_velocity{};

	std::string m_name{ "EnergyBall" };
	static constexpr int EXPLOSION_ANIM_NB_STEPS{ 2 };
	const std::array<sf::IntRect, EXPLOSION_ANIM_NB_STEPS> m_animRects{ sf::IntRect{ 0,0,21,22 }, { 21,0,21,22 }};
	int m_explosionAnimStep{ 0 };

	sf::Clock m_animClock{};
};

#endif

