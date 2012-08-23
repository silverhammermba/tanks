#ifndef TREAD_H_
#define TREAD_H_

class Tread : public Entity
{
	float force;
	float max_force;

	b2v lateral_vel();
	b2v forward_vel();
	void update_friction();
public:
	Tread(const sf::Texture & texture, const b2v origin, const std::vector<b2FixtureDef*> & fixtures, b2World* world, const b2v pos, float dir);

	entity_t type() const { return TANK; };
	void startContact() {};
	void SetUserData(void* ptr);
	inline b2Body* get_body() const { return body; };
	void update();
	void power(float percent, float mforce);
};

#endif
