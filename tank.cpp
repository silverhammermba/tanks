#include <iostream>
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
	turret.setPosition(chasis.getPosition() + v2f(std::sin(deg2rad(chasis.getRotation())), -std::cos(deg2rad(chasis.getRotation()))) * (chasis.getOrigin().y - middley) * ppm);
	turret.setRotation(chasis.getRotation());
	turret.rotate(turret_dir);
}

Tank::Tank(int joy, b2World* wrld, const v2f & size, const v2f & pos, const sf::Color & clr)
	: chasis(size * ppm), turret(v2f(30.f, 7.f)), debug(v2f(60.f, 1.f))
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

	chasis.setOrigin(size * ppm / 2.0f);
	chasis.setPosition(pos);
	chasis.setFillColor(clr);

	turret.setOrigin(5.f, 3.5f);
	turret.setFillColor(sf::Color(clr.r / 2.f, clr.g / 2.f, clr.b / 2.f));
	set_turret();

	debug.setOrigin(0.f, 0.5f);
	debug.setFillColor(sf::Color(0, 255, 0));

	/***** Box2D *****/

	// structures for creating dynamic boxes
	b2BodyDef bodyDef;
	bodyDef.type = b2_dynamicBody;
	bodyDef.position.Set(0.f, 0.f);

	b2PolygonShape dynamicBox;
	dynamicBox.SetAsBox(size.x / 2.f, size.y / 2.f);

	b2FixtureDef fixtureDef;
	fixtureDef.shape = &dynamicBox;
	fixtureDef.density = 1.0f;
	fixtureDef.friction = 0.3f;

	world = wrld;
	body = world->CreateBody(&bodyDef);
	body->CreateFixture(&fixtureDef);
	// don't need UserData
}

// TODO handle Box2D stuff
Tank::~Tank()
{
	world->DestroyBody(body);
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
	b2Vec2 position = body->GetPosition();
	float angle = body->GetAngle();

	chasis.setPosition(sf::Vector2f(position.x, position.y) * ppm);
	chasis.setRotation(rad2deg(angle));

	set_turret();
}

void Tank::draw_on(sf::RenderWindow & window) const
{
	window.draw(chasis);
	// TODO redo turret with Box2D
	//window.draw(turret);
	window.draw(debug);
}

void Tank::read_controller()
{
	if (sf::Joystick::isConnected(joystick))
	{
		target_left = -sf::Joystick::getAxisPosition(joystick, sf::Joystick::Axis::Y);
		target_right = -sf::Joystick::getAxisPosition(joystick, sf::Joystick::Axis::V);
		target_left = deadzone(target_left, Tank::DEADZONE, 1000.f);
		target_right = deadzone(target_right, Tank::DEADZONE, 1000.f);

		turn = (sf::Joystick::getAxisPosition(joystick, sf::Joystick::Axis::R)
		      - sf::Joystick::getAxisPosition(joystick, sf::Joystick::Axis::Z)) * turret_speed;
	}
}

void Tank::move(float time)
{
	// TODO use these
	/*
	world->ApplyForce(b2Vec2& force, b2Vec2& worldpoint);
	b2Vec2 world->GetWorldPoint(b2Vec2&localpoint);
	b2Vec2 GetWorldVector(b2Vec2& localvector);
	*/

	b2Vec2 lforce = body->GetWorldVector(b2Vec2(target_left, 0.f));
	b2Vec2 rforce = body->GetWorldVector(b2Vec2(target_right, 0.f));
	b2Vec2 ltread = body->GetWorldPoint(b2Vec2(0.f, middley - 0.5f));
	b2Vec2 rtread = body->GetWorldPoint(b2Vec2(0.f, -middley + 0.5f));

	body->ApplyForce(lforce, ltread);
	body->ApplyForce(rforce, rtread);

	std::cerr << "Left tread applying " << lforce.x << "," << lforce.y << " at " << ltread.x << "," << ltread.y << "\n";
	std::cerr << "Right tread applying " << rforce.x << "," << rforce.y << " at " << rtread.x << "," << rtread.y << "\n";
	std::cerr << "\x1b[2F";

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
