#include "tank.hpp"

const float Tank::DEADZONE = 30.f;

void Tank::set_rotation_center(float pos)
{
	sf::FloatRect before = chasis.getGlobalBounds();
	chasis.setOrigin(v2f(middlex, pos));
	sf::FloatRect after = chasis.getGlobalBounds();
	chasis.move(before.left - after.left, before.top - after.top);
}

void Tank::set_turret()
{
	// TODO refactor?
	turret.setPosition(chasis.getPosition() + v2f(std::sin(deg2rad(chasis.getRotation())), -std::cos(deg2rad(chasis.getRotation()))) * (chasis.getOrigin().y - middley));
	turret.setRotation(chasis.getRotation());
	turret.rotate(turret_dir);
}

Tank::Tank(int joy, const v2f & size, const v2f & pos, const sf::Color & clr)
	: chasis(size), turret(v2f(30.f, 7.f)), debug(v2f(60.f, 1.f))
{
	joystick = joy;
	width = size.y;
	middlex = size.x / 2.f;
	middley = size.y / 2.f;
	left = 0;
	right = 0;
	speed = 1.5f;
	turret_dir = 0.f;
	turn = 0.f;
	turret_speed = 0.5f;

	chasis.setOrigin(middlex, middley);
	chasis.setPosition(pos);
	chasis.setFillColor(clr);

	turret.setOrigin(5.f, 3.5f);
	turret.setFillColor(sf::Color(clr.r / 2.f, clr.g / 2.f, clr.b / 2.f));
	set_turret();

	debug.setOrigin(0.f, 0.5f);
	debug.setFillColor(sf::Color(0, 255, 0));
}

Tank::~Tank()
{
}

void Tank::draw_on(sf::RenderWindow & window) const
{
	window.draw(chasis);
	window.draw(turret);
	window.draw(debug);
}

void Tank::read_controller()
{
	if (sf::Joystick::isConnected(joystick))
	{
		left = -sf::Joystick::getAxisPosition(joystick, sf::Joystick::Axis::Y);
		right = -sf::Joystick::getAxisPosition(joystick, sf::Joystick::Axis::V);
		left = deadzone(left, Tank::DEADZONE, 100.f);
		right = deadzone(right, Tank::DEADZONE, 100.f);

		turn = (sf::Joystick::getAxisPosition(joystick, sf::Joystick::Axis::R)
		      - sf::Joystick::getAxisPosition(joystick, sf::Joystick::Axis::Z)) * turret_speed;
	}
}

void Tank::move(float time)
{
	if (left == right)
	{
		set_rotation_center(middley);
		// TODO refactor?
		chasis.move(v2f(std::cos(deg2rad(chasis.getRotation())), std::sin(deg2rad(chasis.getRotation()))) * deg2rad(speed * width * left * time));
	}
	else
	{
		set_rotation_center((width * left) / (left - right));
		chasis.rotate(time * (left - right) * speed);
	}

	turret_dir += turn * time;
	set_turret();

	// possible TODO with just a transform?
	debug.setRotation(chasis.getRotation());
	debug.setPosition(chasis.getPosition());
}
