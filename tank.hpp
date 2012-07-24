#ifndef TANK_H_
#define TANK_H_

#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>
#include <Box2D/Box2D.h>
#include "helpers.hpp"
#include "projectile.hpp"

class Projectile;

class Tank
{
	static const float DEADZONE;
	static const float ACCEL;
	static const float DECEL;
	static const float SPEED;

	int joystick;
	float width;
	float middlex;
	float middley;
	float left;
	float right;
	float target_left;
	float target_right;
	float turret_dir;
	float turn;
	float turret_speed;
	bool firing;
	float shot_speed;
	float shot_size;

	sf::RectangleShape chasis;
	sf::RectangleShape turret;
	sf::RectangleShape debug;

	b2World* world;
	b2Body* body;

	void set_rotation_center(float pos);
	void set_turret();
public:
	Tank(int joy, b2World* wrld, const v2f & size, const v2f & pos, const sf::Color & clr);
	~Tank();

	inline int get_joystick() const { return joystick; };
	inline int is_firing() const { return firing; };

	void bind(sf::Event & event);
	void update();
	void draw_on(sf::RenderWindow & window) const;
	void read_controller();
	void move(float time);
	Projectile *fire();
};

#endif
