#include <iostream>
#include "MathVector.h"
#include "Utilities.h"


MathVector::MathVector()
{
}

MathVector::~MathVector()
{
}

MathVector::MathVector(int angle, float length)
	: m_angle{static_cast<float>(angle)}
	, m_length{length}
{
	updateCartesian();
}

MathVector::MathVector(float dx, float dy)
	: m_dx{dx}
	, m_dy{dy}
{
	updatePolar();
}

void MathVector::add(MathVector& other)
{
	m_dx += other.m_dx;
	m_dy += other.m_dy;
	updatePolar();
}

void MathVector::scale(float factor)
{
	m_length *= factor;
	updateCartesian();
}

void MathVector::setAngle(float angle)
{
	m_angle = angle;
	updateCartesian();
}

void MathVector::updateCartesian()
{
	m_dx = m_length * cos(toRadians(m_angle));
	m_dy = m_length * sin(toRadians(m_angle));
}

void MathVector::updatePolar()
{
	m_length = getVectorLength(sf::Vector2f(m_dx, m_dy ));
	m_angle = toDegrees(atan2(m_dy, m_dx));
}