#ifndef FACTORY_H_
#define FACTORY_H_

#include <string>
#include <yaml-cpp/yaml.h>
#include "helpers.hpp"
#include "tread.hpp"
#include "turret.hpp"
#include "tank.hpp"

namespace YAML
{
	template<> struct convert<b2v>
	{
		static Node encode(const b2v & rhs)
		{
			Node node;
			node.push_back(rhs.x);
			node.push_back(rhs.y);
			return node;
		}

		static bool decode(const Node & node, b2v & rhs)
		{
			if (!node.IsSequence())
				return false;
			if (!node.size() == 2)
				return false;
			
			rhs.x = node[0].as<double>();
			rhs.y = node[1].as<double>();
			return true;
		}
	};
}

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

	class Tank
	{
		b2v chassis_size;
		float density;
		float turret_torque;
	public:
		Tank(const std::string & name);
		~Tank();

		::Tank* produce(int joystick, const b2v & pos) const;
	};
}

#endif
