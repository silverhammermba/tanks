#ifndef CHASSIS_H_
#define CHASSIS_H_

class Chassis : public Entity
{
	b2v turret_mount;
	float motor_mount;
	float turret_speed;
	float tread_mount;
public:
	Chassis(const sf::Texture & texture, const b2v origin, const std::vector<b2FixtureDef*> & fixtures, b2World* world, const b2v & pos, float dir, float mo_mount, const b2v & tu_mount, float tu_speed, float tr_mount);

	entity_t type() const { return TANK; };
	b2v get_turret_mount() { return turret_mount; };
	float get_motor_mount() { return motor_mount; };
	float get_turret_speed() { return turret_speed; };
	float get_tread_mount() { return tread_mount; };
	void startContact() {};
	void SetUserData(void* ptr);
	inline b2Body* get_body() const { return body; };
	void update();
};

#endif
