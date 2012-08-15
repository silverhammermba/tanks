#include "engine.hpp"

Chassis::Chassis(b2World* world, const b2v & pos, const b2v & size, float density, const b2v & tu_mount, float tu_speed, float tr_mount)
	: rect(b2v2v2f(size))
{
	turret_mount = tu_mount;
	tread_mount = tr_mount;
	turret_speed = tu_speed;

	rect.setOrigin(rect.getSize() / 2.0f);
	rect.setFillColor(sf::Color(rand() % 256, rand() % 256, rand() % 256));

	b2BodyDef bodyDef;
	bodyDef.type = b2_dynamicBody;
	bodyDef.position.Set(pos.x, pos.y);

	b2PolygonShape box;
	box.SetAsBox(size.x / 2.f, size.y / 2.f);

	b2FixtureDef fixture;
	fixture.shape = &box;
	fixture.density = density;
	fixture.friction = 0.3f;
	fixture.filter.categoryBits = CATEGORY_TANK;
	fixture.filter.maskBits     = CATEGORY_TANK | CATEGORY_SHOT | CATEGORY_WALL | CATEGORY_SMOKE;

	body = world->CreateBody(&bodyDef);
	body->CreateFixture(&fixture);
}

Chassis::~Chassis()
{
	body->GetWorld()->DestroyBody(body);
}

void Chassis::SetUserData(void* ptr)
{
	body->SetUserData(ptr);
}

void Chassis::update()
{
	rect.setPosition(b2v2v2f(body->GetPosition()));
	rect.setRotation(rad2deg(body->GetAngle()));
}

void Chassis::draw_on(sf::RenderWindow & window) const
{
	window.draw(rect);
}
