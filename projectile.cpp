#include <iostream>
#include "projectile.hpp"

Projectile::Projectile(b2World* world, Tank *own, const b2v pos, float dir, float sp, b2v size)
	: rect(b2v2v2f(size))
{
	std::cerr << "Creating projectile at " << pos.x << "," << pos.y << "\n";
	owner = own;
	speed = sp;

	b2BodyDef bodyDef;
	bodyDef.type = b2_dynamicBody;
	bodyDef.position.Set(pos.x, pos.y);

	b2PolygonShape polygon;
	polygon.SetAsBox(size.x / 2.f, size.y / 2.f);

	b2FixtureDef fixture;
	fixture.shape = &polygon;
	// TODO from the ass
	fixture.density = 10.f;
	fixture.friction = 0.f;
	fixture.filter.categoryBits = CATEGORY_SHOT;
	fixture.filter.maskBits = CATEGORY_TANK | CATEGORY_TURRET | CATEGORY_WALL;

	body = world->CreateBody(&bodyDef);
	body->CreateFixture(&fixture);

	rect.setOrigin(rect.getSize() / 2.f);
	rect.setPosition(b2v2v2f(pos));
	rect.setFillColor(sf::Color(0, 0, 0));

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
