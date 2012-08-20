#ifndef TREAD_H_
#define TREAD_H_

class Tread : public Entity
{
	float force;
	float max_force;

	b2Body* body;
	sf::RectangleShape rect;

	b2v lateral_vel();
	b2v forward_vel();
	void update_friction();
public:
	Tread(b2World* world, b2v size, b2v pos, float density);
	~Tread();

	entity_t type() const { return TANK; };
	void startContact() {};
	void SetUserData(void* ptr);
	inline b2Body* get_body() const { return body; };
	void update();
	void draw_on(sf::RenderWindow & window) const;
	void power(float percent, float mforce);
};

#endif
