#ifndef TURRET_H_
#define TURRET_H_

class Turret : public Entity
{
	float shot_impulse;
public:
	Turret(const sf::Texture & texture, const b2v & origin, const std::vector<b2FixtureDef*> & fixtures, b2World* world, const b2v & pos, float dir, float imp);

	entity_t type() const { return TANK; };
	void startContact() {};
	void SetUserData(void* ptr);
	inline b2Body* get_body() const { return body; };
	inline float get_impulse() const { return shot_impulse; };
	b2v tip() const;
	void recoil();
};

#endif
