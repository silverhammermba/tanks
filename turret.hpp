#ifndef TURRET_H_
#define TURRET_H_

#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>
#include <Box2D/Box2D.h>
#include "helpers.hpp"

class Turret
{
	b2Body* body;
	sf::RectangleShape bodyRect;
	sf::RectangleShape gunRect;
public:
	Turret(b2World* world, b2v pos, const b2v & bodySize, const b2v & gunSize, float gunOffset);
	~Turret();

	inline b2Body* get_body() const { return body; };
	void update();
	void draw_on(sf::RenderWindow & window) const;
};

#endif
