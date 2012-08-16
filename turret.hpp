#ifndef TURRET_H_
#define TURRET_H_

class Turret : public Entity
{
	b2Body* body;
	sf::RectangleShape bodyRect;
	sf::RectangleShape gunRect;
	float shot_impulse;
	float shot_size;
public:
	Turret(b2World* world, b2v pos, const b2v & bodySize, const b2v & gunSize, float gunOffset, float density, float gdensity, float imp, float shsize);
	~Turret();

	entity_t type() const { return TANK; };
	void startContact() {};
	void SetUserData(void* ptr);
	inline b2Body* get_body() const { return body; };
	void update();
	void draw_on(sf::RenderWindow & window) const;
	b2v tip() const;
	Projectile *fire();
};

#endif
