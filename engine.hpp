#ifndef ENGINE_H_
#define ENGINE_H_

#define _USE_MATH_DEFINES
#include <sys/types.h>
#include <dirent.h>
#include <cerrno>
#include <cmath>
#include <cstdlib>
#include <ctime>
#include <fstream>
#include <iostream>
#include <list>
#include <sstream>
#include <string>
#include <SFML/System.hpp>
#include <SFML/Window.hpp>
#include <SFML/Graphics.hpp>
#include <Box2D/Box2D.h>
#include <yaml-cpp/yaml.h>

enum entity_t {TANK, WALL, SHOT};

typedef sf::Vector2f v2f;
typedef b2Vec2 b2v;

#include "entity.hpp"
#include "tread.hpp"
#include "chassis.hpp"
#include "motor.hpp"
#include "projectile.hpp"
#include "turret.hpp"
#include "wall.hpp"
#include "factory.hpp"
#include "tank.hpp"
#include "particle.hpp"

const float ppm = 8.f; // pixels per meter

#define CATEGORY_CHASSIS(n) (1 << (n))
#define CATEGORY_TURRET(n) (1 << ((n) + 4))
const short CATEGORY_CHASSES = 0x0f;
const short CATEGORY_TURRETS = 0xf0;
const short CATEGORY_SHOT = 1 << 8;
const short CATEGORY_WALL = 1 << 9;
const short CATEGORY_SMOKE = 1 << 10;

float rand_f(float max);
float rand_i(int max);
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
