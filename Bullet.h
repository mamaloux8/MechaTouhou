#ifndef BULLET_H
#define BULLET_H

#include "VisibleGameObject.h"
#include "MathVector.h"

class Bullet: 
	public VisibleGameObject
{

public:

	enum BulletType
	{
		straight_shot,
		curved_shot,
		spiraling_shot,
		wavy_shot,
		laser_shot
	};

	enum AngleType
	{
		fixed,
		random,
		aimed
	};

	Bullet();
	Bullet(const Bullet&);
	Bullet(MathVector&);
	Bullet(int, float);
	Bullet(float, float);

	//Je voulais faire plusieurs classes de Bullets pour représenter chaque type et rendre plus explicite la création d'une classe bullet
	//mais ça cause plus de problèmes que ça en règle. Je pourrais aussi faire comme la class Type2 dans BulletPattern, mais ça ralentirait
	//peut-être la classe ??

	//Create a straight shot
	Bullet(float, float, AngleType);
	Bullet(float, float, AngleType, std::string);
	Bullet(float, float, Bullet*);

	//Create a curved shot
	Bullet(float, float, AngleType, float);
	Bullet(float, float, Bullet*, float);

	//Create a wavy shot
	Bullet(float, float, AngleType, float, float);
	Bullet(float, float, Bullet*, float, float);

	//Create a laser shot
	//Bullet(int, float, AngleType, float, float)

	~Bullet();

	void update(float elapsedTime);

	const float getAngle() const { return m_velocity.getAngle(); };
	const float getBaseAngle() const { return m_baseAngle; };

	void setAngle(float angle) { m_velocity.setAngle(angle); };
	void setBaseAngle(float angle) { m_baseAngle = angle; };

	void setVelocity(float vel) { m_velocity.setLength(vel); };
	float getVelocity() const { return m_velocity.getLength(); };

	MathVector getVelocityVector() const { return m_velocity; };

	const AngleType getAngleType() const { return m_angleType; };
	void setAngleType(AngleType type) { m_angleType = type; };

	virtual const BulletType getBulletType() const { return m_type; };

	const sf::Clock getTimer() const { return m_timer; };

	float getAngleVelocity() { return  m_angleVelocity; };

	float getWaveMagnitude() { return m_waveMagnitude; };
	float getWaveFrequency() { return m_waveFrequency; };

	const Bullet* getAngleReference() const { return m_angleReference; };
	bool getHasReference() const { return m_hasReference; };



protected:

	MathVector m_velocity{};
	float m_baseAngle{};

	BulletType m_type{straight_shot};
	AngleType m_angleType{};

	float m_angleVelocity{};

	float m_waveMagnitude{};
	float m_waveFrequency{};

	bool m_hasReference{ false };
	Bullet* m_angleReference{this};

	sf::Clock m_timer{};


	void instantiate();
};

#endif
