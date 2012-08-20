#include "engine.hpp"

Motor::Motor(Chassis & chassis, b2v size, float density, float mforce)
	: rect(b2v2v2f(size))
{
	float offset = chassis.get_motor_mount();
	max_force = mforce;

	b2v off(offset, 0);
	rect.setOrigin(rect.getSize() / 2.f - b2v2v2f(off));
	rect.setFillColor(sf::Color(rand() % 256, rand() % 256, rand() % 256));

	body = chassis.get_body();

	b2PolygonShape box;
	box.SetAsBox(size.x / 2.f, size.y / 2.f, off, 0);

	b2FixtureDef fixture;
	fixture.shape = &box;
	fixture.density = density;
	fixture.friction = 0.3f;
	fixture.filter.categoryBits = CATEGORY_TANK;
	fixture.filter.maskBits     = CATEGORY_TANK | CATEGORY_SHOT | CATEGORY_WALL | CATEGORY_SMOKE;

	body->CreateFixture(&fixture);
}

void Motor::update()
{
	rect.setPosition(b2v2v2f(body->GetPosition()));
	rect.setRotation(rad2deg(body->GetAngle()));
}

void Motor::draw_on(sf::RenderWindow & window) const
{
	window.draw(rect);
}
