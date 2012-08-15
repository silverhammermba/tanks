#ifndef FACTORY_H_
#define FACTORY_H_

#include <string>
#include <fstream>
#include <yaml-cpp/yaml.h>
#include "helpers.hpp"
#include "tread.hpp"
#include "turret.hpp"
#include "chassis.hpp"

class Turret;
class Tread;
class Chassis;

namespace Factory
{
	b2World* world;

	class Tread
	{
		b2v size;
		float max_force;
		float density;
	public:
		Tread(const std::string & name);
		~Tread();

		::Tread* produce(const b2v & pos) const;
	};

	class Turret
	{
		b2v size;
		b2v gun_size;
		float gun_offset;
		float density;
		float gun_density;
		float impulse;
		float shot_size;
	public:
		Turret(const std::string & name);
		~Turret();

		::Turret* produce(const b2v & pos) const;
	};

	class Chassis
	{
		b2v size;
		float density;
		b2v turret_mount;
		float turret_torque;
		float tread_mount;
	public:
		Chassis(const std::string & name);
		~Chassis();

		::Chassis* produce(const b2v & pos) const;
	};
}

#endif