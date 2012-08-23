#include "engine.hpp"

Motor::Motor(const sf::Texture & texture, const b2v origin, const std::vector<b2FixtureDef*> & fixtures, b2World* world, const b2v pos, float dir, float mforce)
	: Entity(texture, origin, fixtures, world, pos, dir)
{
	max_force = mforce;
}

void Motor::update()
{
	sprite.setPosition(b2v2v2f(body->GetPosition()));
	sprite.setRotation(rad2deg(body->GetAngle()));
}
