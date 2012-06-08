#ifndef TANK_H_
#define TANK_H_

#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>
#include "helpers.hpp"

class Tank
{
	int joystick;
	float width;
	float middlex;
	float middley;
	float left;
	float right;
	float speed;
	sf::RectangleShape chasis;
	sf::RectangleShape debug;

	void set_rotation_center(float pos);
public:
	Tank(int joy, const v2f & size, const v2f & pos, const sf::Color & clr);
	~Tank();
	void draw_on(sf::RenderWindow & window) const;
	void update_treads();
	void move(float time);
};

#endif
