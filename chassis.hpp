#ifndef CHASSIS_H_
#define CHASSIS_H_

#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>
#include <Box2D/Box2D.h>
#include "helpers.hpp"
#include "entity.hpp"

class Chassis : public Entity
{
	sf::RectangleShape rect;
	b2Body* body;
public:
	Chassis(b2World* world, const b2v & pos, const b2v & size, float density, const sf::Color & clr);
	~Chassis();

	entity_t type() const { return TANK; };
	void startContact() {};
	void SetUserData(void* ptr);
	inline b2Body* get_body() const { return body; };
	void update();
	void draw_on(sf::RenderWindow & window) const;
};

#endif

