#ifndef TANK_H_
#define TANK_H_

#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>
#include <Box2D/Box2D.h>
#include "helpers.hpp"
#include "entity.hpp"
#include "projectile.hpp"
#include "tread.hpp"
#include "turret.hpp"
#include "chassis.hpp"

class Projectile;
class Turret;

class Tank
{
	static const float DEADZONE;
	static const float ACCEL;
	static const float DECEL;
	static const float SPEED;

	int joystick;
	float middley;
	float left;
	float right;
	float horsepower;
	float turn;
	float turret_speed;
	bool firing;

	sf::RectangleShape chassisRect;
	Tread* ltread;
	Tread* rtread;
	Turret* turret;
	Chassis* chassis;

	b2RevoluteJoint* joint;
public:
	Tank(int joy, b2World* world, const b2v & size, const b2v & pos, const sf::Color & clr, float turtorque, Chassis* chas, Turret* tur, Tread* ltr, Tread *rtr);
	~Tank();

	inline int get_joystick() const { return joystick; };
	inline int is_firing() const { return firing; };
	inline Turret* get_turret() const { return turret; };

	void bind(sf::Event & event);
	void update();
	void draw_on(sf::RenderWindow & window) const;
	void read_controller();
	void move();
	Projectile *fire();
};

#endif
