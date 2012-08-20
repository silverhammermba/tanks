#include "engine.hpp"

const float Tank::DEADZONE = 15.f;
const float Tank::ACCEL = 130.f;
const float Tank::DECEL = 175.f;
const float Tank::SPEED = 1.5f;

Tank::Tank(int joy, b2World* world, const b2v & pos, Factory::Chassis & ch_fact, Factory::Motor & mo_fact, Factory::Turret & tu_fact, Factory::Tread & tr_fact)
{
	chassis = ch_fact.produce(pos);
	motor = mo_fact.produce(*chassis);

	b2v turret_pos = pos + chassis->get_turret_mount();
	turret = tu_fact.produce(turret_pos);

	b2v ltread_pos = pos + b2v(0, chassis->get_tread_mount());
	b2v rtread_pos = pos + b2v(0, -chassis->get_tread_mount());
	ltread = tr_fact.produce(ltread_pos);
	rtread = tr_fact.produce(rtread_pos);

	joystick = joy;
	left = 0.f; // left tread percent
	right = 0.f; // right tread percent
	turn = 0.f;
	turret_speed = chassis->get_turret_speed();
	firing = false;

	// TODO can be passed to constructor?
	/*
	chassis->SetUserData(this);
	turret->SetUserData(this);
	ltread->SetUserData(this);
	rtread->SetUserData(this);
	*/

	b2Body* body = chassis->get_body();
	// attach turret
	b2RevoluteJointDef turretJoint;
	turretJoint.Initialize(body, turret->get_body(), turret_pos);
	// TODO simulate joint friction
	turretJoint.maxMotorTorque = 100000.f;
	turretJoint.motorSpeed = 0.0f;
	turretJoint.enableMotor = true;

	joint = (b2RevoluteJoint*)(world->CreateJoint(&turretJoint));

	// attach treads
	b2WeldJointDef lweld;
	lweld.Initialize(body, ltread->get_body(), ltread_pos);
	world->CreateJoint(&lweld);

	b2WeldJointDef rweld;
	rweld.Initialize(body, rtread->get_body(), rtread_pos);
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
	motor->update();
	turret->update();
	ltread->update();
	rtread->update();
}

void Tank::draw_on(sf::RenderWindow & window) const
{
	ltread->draw_on(window);
	rtread->draw_on(window);
	chassis->draw_on(window);
	motor->draw_on(window);
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
	float max_force = motor->get_max_force();
	ltread->power(left, max_force);
	rtread->power(right, max_force);

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
