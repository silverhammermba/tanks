#include "engine.hpp"

Tread::Tread(b2World* world, b2v size, b2v pos, float mforce, float density)
	: rect(b2v2v2f(size))
{
	// TODO too sluggish
	max_force = mforce;

	b2BodyDef bodyDef;
	bodyDef.type = b2_dynamicBody;
	bodyDef.position.Set(pos.x, pos.y);

	b2PolygonShape polygon;
	polygon.SetAsBox(size.x / 2.f, size.y / 2.f);

	b2FixtureDef fixture;
	fixture.shape = &polygon;
	// 7.48 tons
	fixture.density = density;
	fixture.friction = 0.3;
	fixture.filter.categoryBits = CATEGORY_TANK;
	fixture.filter.maskBits = CATEGORY_TANK | CATEGORY_SHOT | CATEGORY_WALL;

	body = world->CreateBody(&bodyDef);
	body->CreateFixture(&fixture);

	rect.setOrigin(b2v2v2f(size) / 2.0f);
	rect.setPosition(b2v2v2f(pos));
	rect.setFillColor(sf::Color(10, 10, 10));

	std::cerr << "Tread Mass: " << body->GetMass() << "\n";
}

Tread::~Tread()
{
	body->GetWorld()->DestroyBody(body);
}

void Tread::update()
{
	update_friction();

	rect.setPosition(b2v2v2f(body->GetPosition()));
	rect.setRotation(rad2deg(body->GetAngle()));
}

void Tread::draw_on(sf::RenderWindow & window) const
{
	window.draw(rect);
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

void Tread::power(float percent)
{
	force = max_force * percent / 100.f;

	body->ApplyForce(body->GetWorldVector(b2Vec2(force, 0.f)), body->GetWorldCenter());
}
