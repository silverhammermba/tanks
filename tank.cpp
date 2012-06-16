#include "tank.hpp"

const float Tank::DEADZONE = 15.f;
const float Tank::ACCEL = 130.f;
const float Tank::DECEL = 175.f;
const float Tank::SPEED = 1.5f;

void Tank::set_rotation_center(float pos)
{
	sf::FloatRect before = chasis.getGlobalBounds();
	chasis.setOrigin(v2f(middlex, pos));
	sf::FloatRect after = chasis.getGlobalBounds();
	chasis.move(before.left - after.left, before.top - after.top);
}

void Tank::set_turret()
{
	// TODO refactor?, store actual position somewhere?
	turret.setPosition(chasis.getPosition() + v2f(std::sin(deg2rad(chasis.getRotation())), -std::cos(deg2rad(chasis.getRotation()))) * (chasis.getOrigin().y - middley));
	turret.setRotation(chasis.getRotation());
	turret.rotate(turret_dir);
}

Tank::Tank(int joy, const v2f & size, const v2f & pos, const sf::Color & clr)
	: chasis(size), turret(v2f(30.f, 7.f)), debug(v2f(60.f, 1.f))
{
	joystick = joy;
	// TODO calculate sizes rather than store them?
	width = size.y;
	middlex = size.x / 2.f;
	middley = size.y / 2.f;
	left = 0.f;
	right = 0.f;
	target_left = 0.f;
	target_right = 0.f;
	turret_dir = 0.f;
	turn = 0.f;
	turret_speed = 1.f;
	firing = false;
	shot_speed = 250.f;
	shot_size = 3.f;

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

void Tank::bind(sf::Event & event)
{
	if (event.type == sf::Event::JoystickButtonPressed && event.joystickButton.joystickId == joystick)
	{
		switch (event.joystickButton.button)
		{
			case 5:
				firing = true;
		}
	}
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
		target_left = -sf::Joystick::getAxisPosition(joystick, sf::Joystick::Axis::Y);
		target_right = -sf::Joystick::getAxisPosition(joystick, sf::Joystick::Axis::V);
		target_left = deadzone(target_left, Tank::DEADZONE, 100.f);
		target_right = deadzone(target_right, Tank::DEADZONE, 100.f);

		turn = (sf::Joystick::getAxisPosition(joystick, sf::Joystick::Axis::R)
		      - sf::Joystick::getAxisPosition(joystick, sf::Joystick::Axis::Z)) * turret_speed;
	}
}

void Tank::move(float time)
{
	float sgn = sign(target_left - left);
	float acceleration = (sgn * left >= 0 ? Tank::ACCEL : Tank::DECEL);
	float before = left;
	left = left + sgn * acceleration * time;
	left = minmax(before, left, target_left);

	sgn = sign(target_right - right);
	acceleration = (sgn * right >= 0 ? Tank::ACCEL : Tank::DECEL);
	before = right;
	right = right + sgn * acceleration * time;
	right = minmax(before, right, target_right);
	/*
	if (left > 0)
	{
		if (target_left < left)
			left = std::max(left - Tank::DECEL * time, target_left);
		else if (target_left > left)
			left = std::min(left + Tank::ACCEL * time, target_left);
	}
	else if (left < 0)
	{
		if (target_left < left)
			left = std::max(left - Tank::ACCEL * time, target_left);
		else if (target_left > left)
			left = std::min(left + Tank::DECEL * time, target_left);
	}
	else
	{
		if (target_left < left)
			left = std::max(left - Tank::ACCEL * time, target_left);
		else if (target_left > left)
			left = std::min(left + Tank::ACCEL * time, target_left);
	}
	*/

	if (left == right)
	{
		set_rotation_center(middley);
		// TODO refactor?
		chasis.move(v2f(std::cos(deg2rad(chasis.getRotation())), std::sin(deg2rad(chasis.getRotation()))) * deg2rad(Tank::SPEED * width * left * time));
	}
	else
	{
		set_rotation_center((width * left) / (left - right));
		chasis.rotate(time * (left - right) * Tank::SPEED);
	}

	turret_dir += turn * time;
	set_turret();

	// possible TODO with just a transform?
	debug.setRotation(chasis.getRotation());
	debug.setPosition(chasis.getPosition());
}

Projectile *Tank::fire()
{
	firing = false;
	v2f traj(std::cos(deg2rad(turret.getRotation())), std::sin(deg2rad(turret.getRotation())));
	return new Projectile(this, turret.getPosition() + traj * (turret.getSize().x - 5.f), traj, shot_speed, shot_size);
}
