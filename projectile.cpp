#include "engine.hpp"

Projectile::Projectile(const sf::Texture & texture, const b2v & origin, const std::vector<b2FixtureDef*> fixtures, b2World* world, const b2v pos, float dir, Tank *own, float impulse)
	: Entity(texture, origin, fixtures, world, pos, dir)
{
	owner = own;
	should_explode = false;

	b2v fwd_norm = body->GetWorldVector(b2v(1.f, 0.f));
	body->ApplyLinearImpulse(b2v(fwd_norm.x * impulse, fwd_norm.y * impulse), body->GetWorldCenter());
}

// TODO put explosion code in destructor?

void Projectile::startContact()
{
	should_explode = true;
}

void Projectile::explode()
{
	// TODO something awesome?
	std::cerr << "BOOOOM!\n";
}
