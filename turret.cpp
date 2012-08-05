#include <iostream>
#include "turret.hpp"

Turret::Turret(b2World* world, b2v pos, const b2v & bodySize, const b2v & gunSize, float gunOffset) : bodyRect(b2v2v2f(bodySize)), gunRect(b2v2v2f(gunSize))
{
	b2BodyDef bodyDef;
	bodyDef.type = b2_dynamicBody;
	bodyDef.position.Set(pos.x, pos.y);

	b2PolygonShape bodyBox;
	bodyBox.SetAsBox(bodySize.x / 2.f, bodySize.y / 2.f);

	b2FixtureDef bodyFixture;
	bodyFixture.shape = &bodyBox;
	// 11.67 tons
	bodyFixture.density = 976.f;
	bodyFixture.friction = 0.3;
	bodyFixture.filter.categoryBits = CATEGORY_TURRET;
	bodyFixture.filter.maskBits = CATEGORY_TURRET | CATEGORY_SHOT | CATEGORY_WALL;

	b2PolygonShape gunBox;
	gunBox.SetAsBox(gunSize.x / 2.f, gunSize.y / 2.f, b2v(gunSize.x / 2.f + gunOffset, 0.f), 0);

	b2FixtureDef gunFixture;
	gunFixture.shape = &gunBox;
	// total guess...
	gunFixture.density = 218.f;
	gunFixture.friction = 0.3;
	gunFixture.filter.categoryBits = CATEGORY_TURRET;
	gunFixture.filter.maskBits = CATEGORY_TURRET | CATEGORY_WALL;

	body = world->CreateBody(&bodyDef);
	body->CreateFixture(&bodyFixture);
	body->CreateFixture(&gunFixture);

	bodyRect.setOrigin(b2v2v2f(bodySize) / 2.f);
	bodyRect.setFillColor(sf::Color(10, 10, 10));

	gunRect.setOrigin(v2f(-gunOffset * ppm, gunSize.y * ppm / 2.f));
	gunRect.setFillColor(sf::Color(255, 255, 255));

	std::cerr << "Turret Mass: " << body->GetMass() << "\n";
}

Turret::~Turret()
{
	body->GetWorld()->DestroyBody(body);
}

void Turret::update()
{
	bodyRect.setPosition(b2v2v2f(body->GetPosition()));
	bodyRect.setRotation(rad2deg(body->GetAngle()));
	gunRect.setPosition(b2v2v2f(body->GetPosition()));
	gunRect.setRotation(rad2deg(body->GetAngle()));
}

void Turret::draw_on(sf::RenderWindow & window) const
{
	window.draw(bodyRect);
	window.draw(gunRect);
}

b2v Turret::tip() const
{
	// TODO hardcoded for now
	return body->GetWorldPoint(b2v(6.06f, 0.f));
}

