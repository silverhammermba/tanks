#include "engine.hpp"

b2Body* Wall::body;

Wall::Wall(const b2v & size, const b2v & pos, float angle) : rect(b2v2v2f(size))
{
	b2PolygonShape box;
	box.SetAsBox(size.x / 2.f, size.y / 2.f, pos, angle);
	b2FixtureDef fixture;
	fixture.shape = &box;
	fixture.filter.categoryBits = CATEGORY_WALL;
	fixture.filter.maskBits     = CATEGORY_TANK | CATEGORY_TURRET | CATEGORY_SHOT | CATEGORY_SMOKE;

	body->CreateFixture(&fixture);
	body->SetUserData(this);

	rect.setOrigin(rect.getSize() / 2.f);
	rect.setFillColor(sf::Color(100, 100, 100));
	rect.setPosition(b2v2v2f(body->GetPosition() + pos));
	rect.setRotation(rad2deg(angle));
}

void Wall::draw_on(sf::RenderWindow & window) const
{
	window.draw(rect);
}
