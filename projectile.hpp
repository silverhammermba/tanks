#ifndef PROJECTILE_H_
#define PROJECTILE_H_

#include "helpers.hpp"
#include "tank.hpp"

class Tank;

class Projectile
{
	float speed;
	Tank *owner;

	b2Body* body;
	sf::RectangleShape rect;

	b2v velocity;
public:
	Projectile(b2World* world, Tank *own, const b2v pos, float dir, float sp, b2v size);
	~Projectile();

	void update();
	sf::FloatRect getGlobalBounds() const { return rect.getGlobalBounds(); };
	void draw_on(sf::RenderWindow & window) const;
};

#endif
