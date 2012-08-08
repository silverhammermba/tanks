#include <iostream>
#include "projectile.hpp"

Projectile::Projectile(b2World* world, Tank *own, const b2v pos, float dir, float sp, b2v size)
	: rect(b2v2v2f(size))
{
	owner = own;
	speed = sp;

	b2BodyDef bodyDef;
	bodyDef.type = b2_dynamicBody;
	bodyDef.position.Set(pos.x, pos.y);
	bodyDef.angle = dir;

	b2PolygonShape polygon;
	polygon.SetAsBox(size.x / 2.f, size.y / 2.f);

	b2FixtureDef fixture;
	fixture.shape = &polygon;
	// TODO from the ass
	fixture.density = 50.f;
	fixture.friction = 0.f;
	fixture.filter.categoryBits = CATEGORY_SHOT;
	fixture.filter.maskBits = CATEGORY_TANK | CATEGORY_TURRET | CATEGORY_WALL;

	body = world->CreateBody(&bodyDef);
	body->CreateFixture(&fixture);

	rect.setOrigin(rect.getSize() / 2.f);
	rect.setPosition(b2v2v2f(pos));
	rect.setFillColor(sf::Color(0, 0, 0));

	float impulse = 1000.f;
	b2v fwd_norm = body->GetWorldVector(b2v(1.f, 0.f));
	body->ApplyLinearImpulse(b2v(fwd_norm.x * impulse, fwd_norm.y * impulse), body->GetWorldCenter());

	std::cerr << "Shot mass: " << body->GetMass() << "\n";
}

Projectile::~Projectile()
{
	//std::cerr << "Deleting projectile\n";
	body->GetWorld()->DestroyBody(body);
}

void Projectile::update()
{
	rect.setPosition(b2v2v2f(body->GetPosition()));
	rect.setRotation(rad2deg(body->GetAngle()));
}

void Projectile::draw_on(sf::RenderWindow & window) const
{
	window.draw(rect);
}
