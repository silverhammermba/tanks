#include <iostream>
#include "tank.hpp"

const float Tank::DEADZONE = 15.f;
const float Tank::ACCEL = 130.f;
const float Tank::DECEL = 175.f;
const float Tank::SPEED = 1.5f;

Tank::Tank(int joy, b2World* world, const b2v & size, const b2v & pos, const sf::Color & clr, float turtorque, Chassis* chas, Turret* tur, Tread* ltr, Tread *rtr)
{
	chassis = chas;
	turret = tur;
	ltread = ltr;
	rtread = rtr;

	joystick = joy;
	middley = size.y / 2.f; // half the width of the tank
	left = 0.f; // left tread percent
	right = 0.f; // right tread percent
	turn = 0.f;
	turret_speed = 1.f;
	firing = false;

	// TODO can be passed to constructor?
	chassis->SetUserData(this);
	turret->SetUserData(this);
	ltread->SetUserData(this);
	rtread->SetUserData(this);

	b2Body* body = chassis->get_body();
	// attach turret
	b2RevoluteJointDef turretJoint;
	turretJoint.Initialize(body, turret->get_body(), body->GetWorldCenter());
	// TODO simulate joint friction
	turretJoint.maxMotorTorque = turtorque;
	turretJoint.motorSpeed = 0.0f;
	turretJoint.enableMotor = true;

	joint = (b2RevoluteJoint*)(world->CreateJoint(&turretJoint));

	// attach treads
	b2WeldJointDef lweld;
	lweld.Initialize(body, ltread->get_body(), b2v(0.f, size.y / 2.f));
	world->CreateJoint(&lweld);

	b2WeldJointDef rweld;
	rweld.Initialize(body, rtread->get_body(), b2v(0.f, -size.y / 2.f));
	world->CreateJoint(&rweld);
}

Tank::~Tank()
{
	delete chassis;
	delete turret;
	delete ltread;
	delete rtread;
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

void Tank::update()
{
	chassis->update();
	turret->update();
	ltread->update();
	rtread->update();
}

void Tank::draw_on(sf::RenderWindow & window) const
{
	ltread->draw_on(window);
	rtread->draw_on(window);
	chassis->draw_on(window);
}

void Tank::read_controller()
{
	if (sf::Joystick::isConnected(joystick))
	{
		left = -sf::Joystick::getAxisPosition(joystick, sf::Joystick::Axis::Y);
		right = -sf::Joystick::getAxisPosition(joystick, sf::Joystick::Axis::V);
		left = deadzone(left, Tank::DEADZONE, 100.f);
		right = deadzone(right, Tank::DEADZONE, 100.f);

		turn = (sf::Joystick::getAxisPosition(joystick, sf::Joystick::Axis::Z)
		      - sf::Joystick::getAxisPosition(joystick, sf::Joystick::Axis::R));
	}
}

void Tank::move()
{
	ltread->power(left);
	rtread->power(right);

	joint->SetMotorSpeed(turn * turret_speed);
	// TODO need to cancel out momentum/prevent buildup of speed
	if (turn == 0.f)
		joint->SetMaxMotorTorque(1000000.f);
	else
		joint->SetMaxMotorTorque(100000.f);
}

Projectile* Tank::fire()
{
	firing = false;
	return turret->fire();
}
