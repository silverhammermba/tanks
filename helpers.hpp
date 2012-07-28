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

// constrain x to [lb, ub]
template <class T>
T clamp(T lb, T x, T ub)
{
	return (x < lb ? lb : (x > ub ? ub : x));
}

#endif
