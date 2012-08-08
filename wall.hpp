#ifndef WALL_H_
#define WALL_H_

#include <SFML/Graphics.hpp>
#include <Box2D/Box2D.h>
#include "helpers.hpp"
#include "entity.hpp"

class Wall : public Entity
{
	sf::RectangleShape rect;
public:
	static b2Body* body;

	Wall(b2v size, b2v pos, float angle = 0.f);
	~Wall() {};

	entity_t type() const { return WALL; };
	void startContact() {};
	void draw_on(sf::RenderWindow & window) const;
};

#endif
