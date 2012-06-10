#ifndef PROJECTILE_H_
#define PROJECTILE_H_

#include "helpers.hpp"
#include "tank.hpp"

class Tank;

class Projectile
{
	Tank *owner;
	sf::RectangleShape shot;
	v2f trajectory;
public:
	Projectile(Tank *own, const v2f pos, const v2f traj);
	~Projectile();

	void draw_on(sf::RenderWindow & window) const;
	void move(float time);
};

#endif
