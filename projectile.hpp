#ifndef PROJECTILE_H_
#define PROJECTILE_H_

#include <SFML/Graphics.hpp>
#include <Box2D/Box2D.h>
#include "helpers.hpp"
#include "entity.hpp"
#include "tank.hpp"

class Tank;

class Projectile : public Entity
{
	float speed;
	Tank *owner;

	b2Body* body;
	sf::RectangleShape rect;

	b2v velocity;
public:
	bool should_explode;

	Projectile(b2World* world, Tank *own, const b2v pos, float dir, float sp, b2v size);
	~Projectile();

	entity_t type() const { return SHOT; };
	b2v pos() const { return body->GetPosition(); };
	void startContact();
	void explode();
	void update();
	sf::FloatRect getGlobalBounds() const { return rect.getGlobalBounds(); };
	void draw_on(sf::RenderWindow & window) const;
};

#endif
