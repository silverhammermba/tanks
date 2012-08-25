#include "engine.hpp"

Entity::Entity(const sf::Texture & texture, const b2v origin, const std::vector<b2FixtureDef*> & fixtures, b2World* world, const b2v pos, float dir)
{
	sprite.setTexture(texture);
	// TODO flip?
	sprite.scale(2, 2);
	sprite.setOrigin(v2f(sprite.getTexture()->getSize()) / 2.f + b2v2v2f(origin));

	b2BodyDef bodyDef;
	bodyDef.type = b2_dynamicBody;
	bodyDef.position.Set(pos.x, pos.y);
	bodyDef.angle = dir;
	body = world->CreateBody(&bodyDef);

	for (auto fixt = fixtures.begin(); fixt != fixtures.end(); fixt++)
	{
		body->CreateFixture(*fixt);
	}
	body->SetUserData(this);
	std::cerr << "Mass: " << body->GetMass() << "\n";
}

Entity::~Entity()
{
	body->GetWorld()->DestroyBody(body);
}

void Entity::update()
{
	sprite.setPosition(b2v2v2f(body->GetPosition()));
	sprite.setRotation(rad2deg(body->GetAngle()));
}

void Entity::draw_on(sf::RenderWindow & window) const
{
	window.draw(sprite);
}
