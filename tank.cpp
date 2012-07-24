#include <iostream>
#include "tank.hpp"

const float Tank::DEADZONE = 15.f;
const float Tank::ACCEL = 130.f;
const float Tank::DECEL = 175.f;
const float Tank::SPEED = 1.5f;

Tank::Tank(int joy, b2World* wrld, const v2f & size, const v2f & pos, const sf::Color & clr)
	: chasisRect(size * ppm), turretRect(v2f((size.x * 2.f * ppm) / 3.f, (size.y * ppm) / 4.f)), debug(v2f(60.f, 1.f))
{
	joystick = joy;
	middley = size.y / 2.f; // half the width of the tank
	left = 0.f; // left tread force
	right = 0.f; // right tread force
	horsepower = 10.f; // kN of tread force
	turn = 0.f;
	turret_speed = 1.f;
	firing = false;
	shot_speed = 250.f;
	shot_size = 3.f;

	chasisRect.setOrigin(size * ppm / 2.0f);
	chasisRect.setPosition(pos);
	chasisRect.setFillColor(clr);

	// TODO some weirdness about the turret origin...
	turretRect.setOrigin(5.f, 3.5f);
	turretRect.setFillColor(sf::Color(clr.r / 2.f, clr.g / 2.f, clr.b / 2.f));

	debug.setOrigin(0.f, 0.5f);
	debug.setFillColor(sf::Color(0, 255, 0));

	/***** Box2D *****/
	world = wrld;

	// structures for creating dynamic boxes
	b2BodyDef chasisBody;
	chasisBody.type = b2_dynamicBody;
	chasisBody.position.Set(0.f, 0.f);
	chasisBody.linearDamping = 1.5f;
	chasisBody.angularDamping = 4.f;

	b2PolygonShape chasisBox;
	chasisBox.SetAsBox(size.x / 2.f, size.y / 2.f);

	b2FixtureDef chasisFixture;
	chasisFixture.shape = &chasisBox;
	chasisFixture.density = 1.0f;
	chasisFixture.friction = 0.3f;

	chasis = world->CreateBody(&chasisBody);
	chasis->CreateFixture(&chasisFixture);

	b2BodyDef turretBody;
	turretBody.type = b2_dynamicBody;
	turretBody.position.Set(0.f, 0.f);
	// gets weird when turrent tank and turret if too high
	turretBody.angularDamping = 1.f;
	
	b2PolygonShape turretBox;
	turretBox.SetAsBox(size.x / 3.f, size.y / 8.f);

	b2FixtureDef turretFixture;
	turretFixture.shape = &turretBox;
	turretFixture.density = 1.0f;
	turretFixture.friction = 0.3f;

	turret = world->CreateBody(&turretBody);
	turret->CreateFixture(&turretFixture);

	b2RevoluteJointDef turretJoint;
	turretJoint.Initialize(chasis, turret, chasis->GetWorldCenter());
	// simulate joint friction
	turretJoint.maxMotorTorque = 10000.f;
	turretJoint.motorSpeed = 0.0f;
	turretJoint.enableMotor = true;

	joint = (b2RevoluteJoint*)(world->CreateJoint(&turretJoint));
}

Tank::~Tank()
{
	world->DestroyJoint(joint);
	world->DestroyBody(turret);
	world->DestroyBody(chasis);
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
	b2Vec2 position = chasis->GetPosition();
	float angle = chasis->GetAngle();

	chasisRect.setPosition(sf::Vector2f(position.x, position.y) * ppm);
	chasisRect.setRotation(rad2deg(angle));

	position = turret->GetPosition();
	angle = turret->GetAngle();

	turretRect.setPosition(sf::Vector2f(position.x, position.y) * ppm);
	turretRect.setRotation(rad2deg(angle));
}

void Tank::draw_on(sf::RenderWindow & window) const
{
	window.draw(chasisRect);
	window.draw(turretRect);
	window.draw(debug);
}

void Tank::read_controller()
{
	if (sf::Joystick::isConnected(joystick))
	{
		left = -sf::Joystick::getAxisPosition(joystick, sf::Joystick::Axis::Y);
		right = -sf::Joystick::getAxisPosition(joystick, sf::Joystick::Axis::V);
		left = horsepower * 10.f * deadzone(left, Tank::DEADZONE, 100.f);
		right = horsepower * 10.f * deadzone(right, Tank::DEADZONE, 100.f);

		turn = (sf::Joystick::getAxisPosition(joystick, sf::Joystick::Axis::Z)
		      - sf::Joystick::getAxisPosition(joystick, sf::Joystick::Axis::R)) * turret_speed;
	}
}

void Tank::move()
{
	b2Vec2 lforce = chasis->GetWorldVector(b2Vec2(left, 0.f));
	b2Vec2 rforce = chasis->GetWorldVector(b2Vec2(right, 0.f));
	b2Vec2 ltread = chasis->GetWorldPoint(b2Vec2(0.f, middley));
	b2Vec2 rtread = chasis->GetWorldPoint(b2Vec2(0.f, -middley));

	chasis->ApplyForce(lforce, ltread);
	chasis->ApplyForce(rforce, rtread);

	joint->SetMotorSpeed(turn);

	debug.setRotation(chasisRect.getRotation());
	debug.setPosition(chasisRect.getPosition());
}

Projectile *Tank::fire()
{
	firing = false;
	v2f traj(std::cos(deg2rad(turretRect.getRotation())), std::sin(deg2rad(turretRect.getRotation())));
	return new Projectile(this, turretRect.getPosition() + traj * (turretRect.getSize().x - 5.f), traj, shot_speed, shot_size);
}
