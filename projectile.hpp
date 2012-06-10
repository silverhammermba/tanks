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
	float speed;
public:
	Projectile(Tank *own, const v2f pos, const v2f traj, float sp, float size);
	~Projectile();

	sf::FloatRect getGlobalBounds() const { return shot.getGlobalBounds(); };
	void draw_on(sf::RenderWindow & window) const;
	void move(float time);
};

#endif
