#ifndef HELPERS_H_
#define HELPERS_H_

#define _USE_MATH_DEFINES
#include <cmath>
#include <SFML/System.hpp>
#include <Box2D/Box2D.h>

const float ppm = 7.5f; // pixels per meter

enum entity_t {TANK, WALL, SHOT};

const short CATEGORY_TANK = 1 << 0;
const short CATEGORY_WALL = 1 << 1;
const short CATEGORY_GROUND = 1 << 2;
const short CATEGORY_TURRET = 1 << 3;
const short CATEGORY_SHOT = 1 << 4;
const short CATEGORY_PLAYERS = 0xf << 5;
#define CATEGORY_PLAYER(n) (1 << (5 + n))

typedef sf::Vector2f v2f;
typedef b2Vec2 b2v;

float deg2rad(float deg);
float rad2deg(float rad);
float deadzone(float val, float zone, float max);
b2v v2f2b2v(const v2f & v);
v2f b2v2v2f(const b2v & v);

// constrain x to [lb, ub]
template <class T>
T clamp(T lb, T x, T ub)
{
	return (x < lb ? lb : (x > ub ? ub : x));
}

#endif
