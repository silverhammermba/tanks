#include "engine.hpp"

Turret::Turret(const sf::Texture & texture, const b2v & origin, const std::vector<b2FixtureDef*> & fixtures, b2World* world, const b2v & pos, float dir, float imp)
	: Entity(texture, origin, fixtures, world, pos, dir)
{
	shot_impulse = imp;
}

void Turret::SetUserData(void* ptr)
{
	body->SetUserData(ptr);
}

b2v Turret::tip() const
{
	// TODO hardcoded for now
	return body->GetWorldPoint(b2v(6.06f, 0.f));
}

void Turret::recoil()
{
	b2v fwd_norm = body->GetWorldVector(b2v(-1.f, 0.f));
	body->ApplyLinearImpulse(b2v(fwd_norm.x * shot_impulse, fwd_norm.y * shot_impulse), body->GetWorldCenter());
}
