#ifndef PARTICLE_H_
#define PARTICLE_H_

class Particle
{
	float lifetime;
	float period;

	b2Body* body;
	sf::CircleShape circle;
public:
	Particle(b2World* world, const b2v pos, const b2v norm, short category);
	~Particle();

	void update(float time);
	void draw_on(sf::RenderWindow & window) const;
	bool expired() const { return lifetime < 0; };
};

#endif
