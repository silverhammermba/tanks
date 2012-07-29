#include <iostream>
#include "tread.hpp"

Tread::Tread(b2World* world, b2v size, b2v pos) : rect(b2v2v2f(size))
{
	b2BodyDef bodyDef;
	bodyDef.type = b2_dynamicBody;
	bodyDef.position.Set(pos.x, pos.y);

	b2PolygonShape polygon;
	polygon.SetAsBox(size.x / 2.f, size.y / 2.f);

	b2FixtureDef fixture;
	fixture.shape = &polygon;
	// 7.48 tons
	fixture.density = 1383.f;
	fixture.friction = 0.3;
	fixture.filter.categoryBits = CATEGORY_TANK;
	fixture.filter.maskBits = CATEGORY_TANK | CATEGORY_WALL;

	body = world->CreateBody(&bodyDef);
	body->CreateFixture(&fixture);

	rect.setOrigin(b2v2v2f(size) / 2.0f);
	rect.setPosition(b2v2v2f(pos));
	rect.setFillColor(sf::Color(10, 10, 10));

	std::cerr << "Tread Mass: " << body->GetMass() << "\n";
}

Tread::~Tread()
{
	body->GetWorld()->DestroyBody(body);
}

void Tread::update()
{
	update_friction();

	rect.setPosition(b2v2v2f(body->GetPosition()));
	rect.setRotation(rad2deg(body->GetAngle()));
}

void Tread::draw_on(sf::RenderWindow & window) const
{
	window.draw(rect);
}


b2v Tread::lateral_vel()
{
	b2v lat_norm = body->GetWorldVector(b2v(0.f, 1.0f));
	return b2Dot(lat_norm, body->GetLinearVelocity()) * lat_norm;
}

b2v Tread::forward_vel()
{
	b2v fwd_norm = body->GetWorldVector(b2v(0.f, 1.0f));
	return b2Dot(fwd_norm, body->GetLinearVelocity()) * fwd_norm;
}

void Tread::update_friction()
{
	// stop sideways movement
	b2v impulse = body->GetMass() * -lateral_vel();
	body->ApplyLinearImpulse(impulse, body->GetWorldCenter());

	// apply wheel friction
	b2v fwd_norm = forward_vel();
	float fwd_speed = fwd_norm.Normalize();
	float drag = -2 * fwd_speed;
	body->ApplyForce(drag * fwd_norm, body->GetWorldCenter());

	// slow spinning
	body->ApplyAngularImpulse(0.1f * body->GetInertia() * -body->GetAngularVelocity());
}
