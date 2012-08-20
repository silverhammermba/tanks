#include "engine.hpp"

const float Particle::START_SIZE = 0.2f;
const float Particle::MAX_SIZE = 3.f;

Particle::Particle(b2World* world, b2v pos, b2v norm, short category) : circle(START_SIZE * ppm)
{
	lifetime = 0.5f + rand_f(1.5f);
	period = lifetime;

	b2BodyDef bodyDef;
	bodyDef.type = b2_dynamicBody;
	bodyDef.position.Set(pos.x, pos.y);
	bodyDef.angle = (rand_f(2 * M_PI));
	
	b2CircleShape circ;
	circ.m_radius = START_SIZE;

	b2FixtureDef fixture;
	fixture.shape = &circ;
	fixture.density = 50.f;
	fixture.friction = 0.f;
	fixture.filter.categoryBits = CATEGORY_SMOKE;
	fixture.filter.maskBits = CATEGORY_WALL | category;

	body = world->CreateBody(&bodyDef);
	body->CreateFixture(&fixture);
	
	circle.setOrigin(START_SIZE * ppm, START_SIZE * ppm);
	circle.setPosition(b2v2v2f(pos));
	int grey = rand_i(20);
	circle.setFillColor(sf::Color(grey, grey, grey, 255));

	max_size = 1.f + rand_f(MAX_SIZE - 1.f);

	float impulse1 = 5.f + rand_f(15.f);
	float impulse2 = 5.f + rand_f(15.f);
	b2v fwd_norm = body->GetWorldVector(b2v(1.f, 0.f));
	body->ApplyLinearImpulse(b2v(fwd_norm.x * impulse1 + norm.x * impulse2, fwd_norm.y * impulse1 + norm.y * impulse2) + norm, body->GetWorldCenter());
}

Particle::~Particle()
{
	body->GetWorld()->DestroyBody(body);
}

void Particle::update(float time)
{
	circle.setPosition(b2v2v2f(body->GetPosition()));

	float impulse = 1.f;
	float dir = rand_f(2 * M_PI);
	body->ApplyLinearImpulse(b2v(cos(dir) * impulse, sin(dir) * impulse), body->GetWorldCenter());

	lifetime -= time;

	circle.setRadius((START_SIZE + max_size * sin(M_PI * lifetime / period)) * ppm);
	circle.setOrigin(circle.getRadius(), circle.getRadius());

	sf::Color color = circle.getFillColor();
	circle.setFillColor(sf::Color(color.r, color.g, color.b, 255 * std::sqrt(lifetime / period)));
}

void Particle::draw_on(sf::RenderWindow & window) const
{
	window.draw(circle);
}
