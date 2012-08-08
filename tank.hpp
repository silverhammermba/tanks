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

class Projectile;

class Tank : public Entity
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
	float shot_speed;
	float shot_size;

	sf::RectangleShape chassisRect;
	sf::RectangleShape debug;
	Tread ltread;
	Tread rtread;
	Turret turret;

	b2Body* chassis;
	b2RevoluteJoint* joint;
public:
	Tank(int joy, b2World* wrld, b2Body* ground, const b2v & size, const b2v & pos, const sf::Color & clr);
	~Tank();

	entity_t type() const { return TANK; };
	void startContact();
	inline int get_joystick() const { return joystick; };
	inline int is_firing() const { return firing; };
	inline const Turret & get_turret() const { return turret; };

	void bind(sf::Event & event);
	void update();
	void draw_on(sf::RenderWindow & window) const;
	void read_controller();
	void move();
	Projectile *fire();
};

#endif
