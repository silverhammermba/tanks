#ifndef TANK_H_
#define TANK_H_

#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>
#include "helpers.hpp"


class Tank
{
	static const float DEADZONE;

	int joystick;
	float width;
	float middlex;
	float middley;
	float left;
	float right;
	float speed;
	float turret_dir;
	float turn;
	float turret_speed;
	sf::RectangleShape chasis;
	sf::RectangleShape turret;
	sf::RectangleShape debug;

	void set_rotation_center(float pos);
	void set_turret();
public:
	Tank(int joy, const v2f & size, const v2f & pos, const sf::Color & clr);
	~Tank();
	void draw_on(sf::RenderWindow & window) const;
	void read_controller();
	void move(float time);
};

#endif
