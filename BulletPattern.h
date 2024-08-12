#ifndef BULLET_PATTERN_H
#define BULLET_PATTERN_H

#include "Bullet.h"
#include <variant>

class BulletPattern
{
public:
	
	enum AngleReference
	{
		ignoreReference,
		fixReference,
		useReference,
	};

	enum Type
	{
		singleShot,
		circle,
	};

	class Type2
	{
	public:
		Type2()
		{
		};
		virtual const Type& getType() const = 0;
		virtual const int getNbBullets() const = 0;
	};
	class Circle
		: public Type2
	{
	public:
		Circle(int nbBullets)
		{
			m_nbBullets = nbBullets;
		}
		const Type& getType() const { return m_type; };
		const int getNbBullets() const { return m_nbBullets; };

	private:
		int m_nbBullets{};
		const Type m_type{ circle };
	};

	BulletPattern(Bullet*);
	BulletPattern(Bullet*, AngleReference, float);
	BulletPattern(Bullet*, Type2*);
	BulletPattern(Bullet*, float);
	~BulletPattern();

	void spawnCircle(int);
	void spawnSingleShot();

	void setBulletPosition(sf::Vector2f);
	void setBulletAngle();
	void setBulletAngle(float angle) { m_bullet->setAngle(angle); };
	const Type getType() const { return m_type; };
	const Type2* getType2() const { return m_type2; };

	int getNbBullets() { return m_nbBullets; };

	int getRepeatCounter() const { return m_repeatCounter; };
	BulletPattern* setRepeatCounter(int count, float acceleration) { m_repeatCounter = count; m_acceleration = acceleration;  return this; };

	void rotate(float elapsedTime);
	BulletPattern* setSpeed(float speed) { m_bullet->setVelocity(speed); return this; };
	float getSpeed() const { return m_bullet->getVelocity(); };
	float getAcceleration() const { return m_acceleration; };
	float getAngle() const { return m_bullet->getAngle(); };
	//A enlever, sert plus a rien
	bool getHasReference() const { return m_bullet->getHasReference(); };

	AngleReference getAngleReference() const { return m_angleReference; };
	float getAngleOffset() { return m_angleOffset; };

	MathVector getBaseVelocity() const { return m_bulletBaseVelocity; };

private:
	Bullet* m_bullet;

	Type m_type{};
	Type2* m_type2{};

	int m_nbBullets{};
	int m_repeatCounter{ 0 };
	float m_rotation{ 0.f };
	float m_acceleration{};

	MathVector m_bulletBaseVelocity{};
	AngleReference m_angleReference{ ignoreReference };
	float m_angleOffset{};
	 

	VisibleGameObject* m_player;


};

#endif
