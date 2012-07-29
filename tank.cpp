#include <iostream>
#include "tank.hpp"

const float Tank::DEADZONE = 15.f;
const float Tank::ACCEL = 130.f;
const float Tank::DECEL = 175.f;
const float Tank::SPEED = 1.5f;

Tank::Tank(int joy, b2World* world, b2Body* ground, const b2v & size, const b2v & pos, const sf::Color & clr)
	: chassisRect(b2v2v2f(size)), ltread(world, b2v(size.x, 0.68f), b2v(0.f, size.y / 2.f)), rtread(world, b2v(size.x, 0.68f), b2v(0.f, -size.y / 2.f)),
	  turret(world, pos, b2v(3.15f, 3.15f), b2v(5.24f, 0.3f), 0.82f), debug(v2f(60.f, 1.f))
{
	joystick = joy;
	middley = size.y / 2.f; // half the width of the tank
	left = 0.f; // left tread force
	right = 0.f; // right tread force
	horsepower = 1.f; // kN of tread force
	turn = 0.f;
	turret_speed = 10.f;
	firing = false;
	shot_speed = 250.f;
	shot_size = 3.f;

	chassisRect.setOrigin(b2v2v2f(size) / 2.0f);
	chassisRect.setFillColor(clr);

	debug.setOrigin(0.f, 0.5f);
	debug.setFillColor(sf::Color(0, 255, 0));

	/***** Box2D *****/

	// structures for creating dynamic boxes
	b2BodyDef chassisBody;
	chassisBody.type = b2_dynamicBody;
	chassisBody.position.Set(0.f, 0.f);

	b2PolygonShape chassisBox;
	chassisBox.SetAsBox(size.x / 2.f, size.y / 2.f);

	b2FixtureDef chassisFixture;
	chassisFixture.shape = &chassisBox;
	// 22.34 tons
	chassisFixture.density = 771.f;
	chassisFixture.friction = 0.3f;
	// collides with other chasses
	chassisFixture.filter.categoryBits = CATEGORY_TANK;
	chassisFixture.filter.maskBits     = CATEGORY_TANK | CATEGORY_WALL;

	chassis = world->CreateBody(&chassisBody);
	chassis->CreateFixture(&chassisFixture);

	b2RevoluteJointDef turretJoint;
	turretJoint.Initialize(chassis, turret.get_body(), chassis->GetWorldCenter());
	// TODO simulate joint friction
	turretJoint.maxMotorTorque = 1000.f;
	turretJoint.motorSpeed = 0.0f;
	turretJoint.enableMotor = true;

	joint = (b2RevoluteJoint*)(world->CreateJoint(&turretJoint));

	b2WeldJointDef lweld;
	lweld.Initialize(chassis, ltread.get_body(), b2v(0.f, size.y / 2.f));
	world->CreateJoint(&lweld);

	b2WeldJointDef rweld;
	rweld.Initialize(chassis, rtread.get_body(), b2v(0.f, -size.y / 2.f));
	world->CreateJoint(&rweld);

	std::cerr << "Tank Mass: " << chassis->GetMass() << "\n";
}

Tank::~Tank()
{
	chassis->GetWorld()->DestroyBody(chassis);
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

	turret.update();

	ltread.update();
	rtread.update();
}

void Tank::draw_on(sf::RenderWindow & window) const
{
	ltread.draw_on(window);
	rtread.draw_on(window);
	window.draw(chassisRect);
	turret.draw_on(window);
}

void Tank::read_controller()
{
	if (sf::Joystick::isConnected(joystick))
	{
		left = -sf::Joystick::getAxisPosition(joystick, sf::Joystick::Axis::Y);
		right = -sf::Joystick::getAxisPosition(joystick, sf::Joystick::Axis::V);
		left = deadzone(left, Tank::DEADZONE, 100.f);
		right = deadzone(right, Tank::DEADZONE, 100.f);

		left *= horsepower * 10.f;
		right *= horsepower * 10.f;

		turn = (sf::Joystick::getAxisPosition(joystick, sf::Joystick::Axis::Z)
		      - sf::Joystick::getAxisPosition(joystick, sf::Joystick::Axis::R)) * turret_speed;
	}
}

void Tank::move()
{
	b2Vec2 lforce = chassis->GetWorldVector(b2Vec2(left, 0.f));
	b2Vec2 rforce = chassis->GetWorldVector(b2Vec2(right, 0.f));
	b2Vec2 ltrd = chassis->GetWorldPoint(b2Vec2(0.f, middley));
	b2Vec2 rtrd = chassis->GetWorldPoint(b2Vec2(0.f, -middley));

	chassis->ApplyForce(lforce, ltrd);
	chassis->ApplyForce(rforce, rtrd);

	joint->SetMotorSpeed(turn);

	debug.setRotation(chassisRect.getRotation());
	debug.setPosition(chassisRect.getPosition());
}

/*
Projectile *Tank::fire()
{
	firing = false;
	v2f traj(std::cos(deg2rad(turretRect.getRotation())), std::sin(deg2rad(turretRect.getRotation())));
	return new Projectile(this, turretRect.getPosition() + traj * (turretRect.getSize().x - 5.f), traj, shot_speed, shot_size);
}
*/
