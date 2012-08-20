#ifndef MOTOR_H_
#define MOTOR_H_

class Motor : public Entity
{
	float max_force;

	b2Body* body;
	sf::RectangleShape rect;
public:
	Motor(Chassis & chassis, b2v size, float density, float mforce);
	~Motor() {};

	entity_t type() const { return TANK; }
	void startContact() {};
	void SetUserData(void* ptr);
	inline float get_max_force() const { return max_force; };
	inline b2Body* get_body() const { return body; };
	void update();
	void draw_on(sf::RenderWindow & window) const;
};

#endif
