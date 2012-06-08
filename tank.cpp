#include "tank.hpp"

void Tank::set_rotation_center(float pos)
{
	sf::FloatRect before = chasis.getGlobalBounds();
	chasis.setOrigin(v2f(middlex, pos));
	sf::FloatRect after = chasis.getGlobalBounds();
	chasis.move(before.left - after.left, before.top - after.top);
}

Tank::Tank(int joy, const v2f & size, const v2f & pos, const sf::Color & clr)
	: chasis(size), debug(v2f(60.f, 1.f))
{
	joystick = joy;
	width = size.y;
	middlex = size.x / 2.f;
	middley = size.y / 2.f;
	left = 0;
	right = 0;
	speed = 2.f;

	chasis.setOrigin(middlex, middley);
	chasis.setPosition(pos);
	chasis.setFillColor(clr);

	debug.setOrigin(0.f, 0.5f);
	debug.setFillColor(sf::Color(0, 255, 0));
}

Tank::~Tank()
{
}

void Tank::draw_on(sf::RenderWindow & window) const
{
	window.draw(chasis);
	window.draw(debug);
}

void Tank::update_treads()
{
	if (sf::Joystick::isConnected(joystick))
	{
		left = -sf::Joystick::getAxisPosition(joystick, sf::Joystick::Axis::Y);
		right = -sf::Joystick::getAxisPosition(joystick, sf::Joystick::Axis::V);
		// TODO improve deadzone calculation
		if (std::abs(left) < 30.f)
			left = 0;
		if (std::abs(right) < 30.f)
			right = 0;
	}
}

void Tank::move(float time)
{
	if (left == right)
	{
		set_rotation_center(middley);
		// TODO refactor
		chasis.move(v2f(std::cos(deg2rad(chasis.getRotation())), std::sin(deg2rad(chasis.getRotation()))) * deg2rad(speed * width * left * time));
	}
	else
	{
		set_rotation_center((width * left) / (left - right));
		chasis.rotate(time * (left - right) * speed);
	}
	// possible TODO with just a transform?
	debug.setRotation(chasis.getRotation());
	debug.setPosition(chasis.getPosition());
}
