#include "engine.hpp"

const float Tank::DEADZONE = 15.f;
const float Tank::ACCEL = 130.f;
const float Tank::DECEL = 175.f;
const float Tank::SPEED = 1.5f;

Tank::Tank(int joy, b2World* world, const b2v & pos, Factory::Chassis & ch_fact, Factory::Motor & mo_fact, Factory::Turret & tu_fact, Factory::Tread & tr_fact, Factory::Projectile & mg_fact)
{
	float dir = 0.f;
	chassis = ch_fact.produce(pos, dir);
	b2v motor_pos = pos + b2v(chassis->get_motor_mount(), 0);
	motor = mo_fact.produce(motor_pos, dir);

	b2v turret_pos = pos + chassis->get_turret_mount();
	turret = tu_fact.produce(turret_pos, dir);

	b2v ltread_pos = pos + b2v(0, chassis->get_tread_mount());
	b2v rtread_pos = pos + b2v(0, -chassis->get_tread_mount());
	ltread = tr_fact.produce(ltread_pos, dir);
	rtread = tr_fact.produce(rtread_pos, dir);

	magazine = &mg_fact;

	components.push_back(ltread);
	components.push_back(rtread);
	components.push_back(chassis);
	components.push_back(motor);
	components.push_back(turret);

	joystick = joy;
	left = 0.f; // left tread percent
	right = 0.f; // right tread percent
	turn = 0.f;
	turret_speed = chassis->get_turret_speed();
	firing = false;

	b2Body* body = chassis->get_body();
	// attach turret
	b2RevoluteJointDef turretJoint;
	turretJoint.Initialize(body, turret->get_body(), turret_pos);
	// TODO simulate joint friction
	turretJoint.maxMotorTorque = 100000.f;
	turretJoint.motorSpeed = 0.0f;
	turretJoint.enableMotor = true;

	joint = (b2RevoluteJoint*)(world->CreateJoint(&turretJoint));

	// attach motor
	b2WeldJointDef mweld;
	mweld.Initialize(body, motor->get_body(), motor_pos);
	world->CreateJoint(&mweld);

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
	delete motor;
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
	for (auto c = components.begin(); c != components.end(); c++)
		(*c)->update();
}

void Tank::draw_on(sf::RenderWindow & window) const
{
	for (auto c = components.begin(); c != components.end(); c++)
		(*c)->draw_on(window);
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
	turret->recoil();
	return magazine->produce(turret->tip(), turret->get_body()->GetAngle(), this, turret->get_impulse());
}
