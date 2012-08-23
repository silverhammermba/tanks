#include "engine.hpp"

// TODO nicer way to do this?
Tread::Tread(const sf::Texture & texture, const b2v origin, const std::vector<b2FixtureDef*> & fixtures, b2World* world, const b2v pos, float dir)
	: Entity(texture, origin, fixtures, world, pos, dir) {}

void Tread::update()
{
	update_friction();
	Entity::update();
}

b2v Tread::lateral_vel()
{
	b2v lat_norm = body->GetWorldVector(b2v(0.f, 1.f));
	return b2Dot(lat_norm, body->GetLinearVelocity()) * lat_norm;
}

b2v Tread::forward_vel()
{
	b2v fwd_norm = body->GetWorldVector(b2v(1.f, 0.f));
	return b2Dot(fwd_norm, body->GetLinearVelocity()) * fwd_norm;
}

void Tread::update_friction()
{
	// TODO breaking is currently wonky
	// stop sideways movement
	b2v impulse = body->GetMass() * -lateral_vel();
	body->ApplyLinearImpulse(impulse, body->GetWorldCenter());

	// TODO get accel force from motor, decel force from treads
	// apply wheel friction
	b2v fwd_norm = forward_vel();
	float fwd_speed = fwd_norm.Normalize();
	// TODO perhaps too strong...
	float drag = -fwd_speed * max_force * (max_force - std::abs(force)) / max_force;
	body->ApplyForce(drag * fwd_norm, body->GetWorldCenter());

	// slow spinning
	body->ApplyAngularImpulse(0.08f * body->GetInertia() * -body->GetAngularVelocity());
}

void Tread::SetUserData(void* ptr)
{
	body->SetUserData(ptr);
}

void Tread::power(float percent, float mforce)
{
	max_force = mforce;
	force = max_force * percent / 100.f;

	body->ApplyForce(body->GetWorldVector(b2Vec2(force, 0.f)), body->GetWorldCenter());
}
