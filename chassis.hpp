#ifndef CHASSIS_H_
#define CHASSIS_H_

class Chassis : public Entity
{
	sf::RectangleShape rect;
	b2Body* body;
	b2v turret_mount;
	float turret_speed;
	float tread_mount;
public:
	Chassis(b2World* world, const b2v & pos, const b2v & size, float density, const b2v & tu_mount, float tu_speed, float tr_mount);
	~Chassis();

	entity_t type() const { return TANK; };
	b2v get_turret_mount() { return turret_mount; };
	float get_turret_speed() { return turret_speed; };
	float get_tread_mount() { return tread_mount; };
	void startContact() {};
	void SetUserData(void* ptr);
	inline b2Body* get_body() const { return body; };
	void update();
	void draw_on(sf::RenderWindow & window) const;
};

#endif
