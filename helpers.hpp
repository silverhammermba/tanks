#ifndef HELPERS_H_
#define HELPERS_H_

#define _USE_MATH_DEFINES
#include <cmath>
#include <SFML/System.hpp>

const float ppm = 2.5f; // pixels per meter

typedef sf::Vector2f v2f;

float deg2rad(float deg);
float rad2deg(float rad);
float deadzone(float val, float zone, float max);
int sign(float x);
float minmax(float before, float amount, float target);

// constrain x to [lb, ub]
template <class T>
T clamp(T lb, T x, T ub)
{
	return (x < lb ? lb : (x > ub ? ub : x));
}

// find distance between two points as sf::Vector2<T>
template <class T>
float v2dist(sf::Vector2<T> v1, sf::Vector2<T> v2)
{
	return std::sqrt((float)((v1.x - v2.x) * (v1.x - v2.x) + (v1.y - v2.y) * (v1.y - v2.y)));
}

// find square of distance between two points as sf::Vector2<T>
template <class T>
float v2dist2(sf::Vector2<T> v1, sf::Vector2<T> v2)
{
	return (float)((v1.x - v2.x) * (v1.x - v2.x) + (v1.y - v2.y) * (v1.y - v2.y));
}

#endif
