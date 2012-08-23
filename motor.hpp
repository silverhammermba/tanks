#ifndef MOTOR_H_
#define MOTOR_H_

class Motor : public Entity
{
	float max_force;
public:
	Motor(const sf::Texture & texture, const b2v origin, const std::vector<b2FixtureDef*> & fixtures, b2World* world, const b2v pos, float dir, float mforce);

	entity_t type() const { return TANK; }
	void startContact() {};
	void SetUserData(void* ptr);
	inline float get_max_force() const { return max_force; };
	inline b2Body* get_body() const { return body; };
	void update();
};

#endif
