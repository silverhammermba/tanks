#include "engine.hpp"

void operator >> (const YAML::Node& node, b2v & v)
{
	node[0] >> v.x;
	node[1] >> v.y;
}

namespace Factory
{
	b2World* world;

	Tread::Tread(const std::string & name)
	{
		std::cerr << "Loading " << name << "...\n";
		std::ifstream fin(name);
		YAML::Parser parser(fin);

		YAML::Node def;
		if (parser.GetNextDocument(def))
		{
			def["size"] >> size;
			def["mass"] >> density;
			density /= size.x * size.y;
			def["force"] >> max_force;
		}
	}

	Tread::~Tread() {}

	::Tread* Tread::produce(const b2v & pos) const
	{
		return new ::Tread(world, size, pos, max_force, density);
	}

	Turret::Turret(const std::string & name)
	{
		std::cerr << "Loading " << name << "...\n";
		std::ifstream fin(name);
		YAML::Parser parser(fin);

		YAML::Node def;
		if (parser.GetNextDocument(def))
		{
			def["size"] >> size;
			def["mass"] >> density;
			density /= size.x * size.y;
			def["gun_size"] >> gun_size;
			def["gun_mass"] >> gun_density;
			gun_density /= size.x * size.y;
			def["gun_offset"] >> gun_offset;
			def["impulse"] >> impulse;
			def["shot_size"] >> shot_size;
		}
	}

	Turret::~Turret() {}

	::Turret* Turret::produce(const b2v & pos) const
	{
		return new ::Turret(world, pos, size, gun_size, gun_offset, density, gun_density, impulse, shot_size);
	}

	Chassis::Chassis(const std::string & name)
	{
		std::cerr << "Loading " << name << "...\n";
		std::ifstream fin(name);
		YAML::Parser parser(fin);

		YAML::Node def;
		if (parser.GetNextDocument(def))
		{
			def["size"] >> size;
			def["mass"] >> density;
			density /= size.x * size.y;
			def["turret_mount"] >> turret_mount;
			def["turret_speed"] >> turret_speed;
			def["tread_mount"] >> tread_mount;
		}
	}

	Chassis::~Chassis() {}

	::Chassis* Chassis::produce(const b2v & pos) const
	{
		return new ::Chassis(world, pos, size, density, turret_mount, turret_speed, tread_mount);
	}
}
