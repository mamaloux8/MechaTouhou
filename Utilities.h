#ifndef UTILITIES_H
#define UTILITIES_H

#include <SFML/System.hpp>

template <typename T>
float getVectorLength(sf::Vector2<T> vec)
{
	return static_cast<float>(sqrt(pow(vec.x, 2) + pow(vec.y, 2)));
}

template <typename T>
float toRadians(T angle)
{
	return angle * 3.14159265f / 180;
}

template <typename T>
float toDegrees(T angle)
{
	return angle * 180 / 3.14159265f;
}

#endif