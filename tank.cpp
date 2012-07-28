#include <iostream>
#include "tank.hpp"

const float Tank::DEADZONE = 15.f;
const float Tank::ACCEL = 130.f;
const float Tank::DECEL = 175.f;
const float Tank::SPEED = 1.5f;

Tank::Tank(int joy, b2World* wrld, b2Body* ground, const b2v & size, const b2v & pos, const sf::Color & clr)
	: chassisRect(b2v2v2f(size)), turretRect(v2f(size.x * ppm * 3.f / 4.f, size.y * ppm / 4.f)), debug(v2f(60.f, 1.f))
{
	joystick = joy;
	middley = size.y / 2.f; // half the width of the tank
	left = 0.f; // left tread force
	right = 0.f; // right tread force
	horsepower = 1.f; // kN of tread force
	turn = 0.f;
	turret_speed = 1.f;
	firing = false;
	shot_speed = 250.f;
	shot_size = 3.f;

	chassisRect.setOrigin(b2v2v2f(size) / 2.0f);
	chassisRect.setPosition(b2v2v2f(pos));
	chassisRect.setFillColor(clr);

	turretRect.setOrigin(size.x * ppm * 3.f / 8.f, size.y * ppm / 8.f);
	//turretRect.setOrigin(size.y * ppm / 8.f, size.y * ppm / 8.f);
	turretRect.setFillColor(sf::Color(clr.r / 2.f, clr.g / 2.f, clr.b / 2.f));

	debug.setOrigin(0.f, 0.5f);
	debug.setFillColor(sf::Color(0, 255, 0));

	/***** Box2D *****/
	world = wrld;

	// structures for creating dynamic boxes
	b2BodyDef chassisBody;
	chassisBody.type = b2_dynamicBody;
	chassisBody.position.Set(0.f, 0.f);

	b2PolygonShape chassisBox;
	chassisBox.SetAsBox(size.x / 2.f, size.y / 2.f);

	b2FixtureDef chassisFixture;
	chassisFixture.shape = &chassisBox;
	chassisFixture.density = 1.0f;
	chassisFixture.friction = 0.3f;
	// collides with other chasses
	chassisFixture.filter.categoryBits = 0x0001;
	chassisFixture.filter.maskBits     = 0x0001;

	chassis = world->CreateBody(&chassisBody);
	chassis->CreateFixture(&chassisFixture);

	b2BodyDef turretBody;
	turretBody.type = b2_dynamicBody;
	turretBody.position.Set(0.f, 0.f);
	
	b2PolygonShape turretBox;
	turretBox.SetAsBox(size.x * 3.f / 8.f, size.y / 8.f);

	b2FixtureDef turretFixture;
	turretFixture.shape = &turretBox;
	turretFixture.density = 1.0f;
	turretFixture.friction = 0.3f;

	turret = world->CreateBody(&turretBody);
	turret->CreateFixture(&turretFixture);

	b2RevoluteJointDef turretJoint;
	turretJoint.Initialize(chassis, turret, chassis->GetWorldCenter());
	// simulate joint friction
	turretJoint.maxMotorTorque = 10000.f;
	turretJoint.motorSpeed = 0.0f;
	turretJoint.enableMotor = true;

	joint = (b2RevoluteJoint*)(world->CreateJoint(&turretJoint));

	b2FrictionJointDef frictionJoint;
	frictionJoint.Initialize(chassis, ground, b2Vec2(0.f, 0.f));
	//frictionJoint.localAnchorA = b2Vec2(0.f, 0.f);
	//frictionJoint.localAnchorB = b2Vec2(0.f, 0.f);
	frictionJoint.maxForce = 100.0f;
	frictionJoint.maxTorque = 10000.0f;

	friction = (b2FrictionJoint*)(world->CreateJoint(&frictionJoint));
}

Tank::~Tank()
{
	world->DestroyJoint(friction);
	world->DestroyJoint(joint);
	world->DestroyBody(turret);
	world->DestroyBody(chassis);
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
	chassisRect.setPosition(b2v2v2f(chassis->GetPosition()));
	chassisRect.setRotation(rad2deg(chassis->GetAngle()));

	turretRect.setPosition(b2v2v2f(turret->GetPosition()));
	turretRect.setRotation(rad2deg(turret->GetAngle()));
}

void Tank::draw_on(sf::RenderWindow & window) const
{
	window.draw(chassisRect);
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
	b2Vec2 lforce = chassis->GetWorldVector(b2Vec2(left, 0.f));
	b2Vec2 rforce = chassis->GetWorldVector(b2Vec2(right, 0.f));
	b2Vec2 ltread = chassis->GetWorldPoint(b2Vec2(0.f, middley));
	b2Vec2 rtread = chassis->GetWorldPoint(b2Vec2(0.f, -middley));

	chassis->ApplyForce(lforce, ltread);
	chassis->ApplyForce(rforce, rtread);

	joint->SetMotorSpeed(turn);

	debug.setRotation(chassisRect.getRotation());
	debug.setPosition(chassisRect.getPosition());
}

Projectile *Tank::fire()
{
	firing = false;
	v2f traj(std::cos(deg2rad(turretRect.getRotation())), std::sin(deg2rad(turretRect.getRotation())));
	return new Projectile(this, turretRect.getPosition() + traj * (turretRect.getSize().x - 5.f), traj, shot_speed, shot_size);
}
