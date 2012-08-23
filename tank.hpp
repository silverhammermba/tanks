#ifndef TANK_H_
#define TANK_H_

class Tank
{
	static const float DEADZONE;
	static const float ACCEL;
	static const float DECEL;
	static const float SPEED;

	int joystick;
	float left;
	float right;
	float horsepower;
	float turn;
	float turret_speed;
	bool firing;

	std::vector<Entity*> components;
	Tread* ltread;
	Tread* rtread;
	Turret* turret;
	Chassis* chassis;
	Motor* motor;

	Factory::Projectile* magazine;

	b2RevoluteJoint* joint;
public:
	Tank(int joy, b2World* world, const b2v & pos, Factory::Chassis & ch_fact, Factory::Motor & mo_fact, Factory::Turret & tu_fact, Factory::Tread & tr_fact, Factory::Projectile & mg_fact);
	~Tank();

	inline int get_joystick() const { return joystick; };
	inline int is_firing() const { return firing; };
	inline Turret* get_turret() const { return turret; };

	void bind(sf::Event & event);
	void update();
	void draw_on(sf::RenderWindow & window) const;
	void read_controller();
	void move();
	Projectile *fire();
};

#endif
