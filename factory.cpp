#include "engine.hpp"

void operator >> (const YAML::Node& node, b2v & v)
{
	node[0] >> v.x;
	node[1] >> v.y;
}

namespace Factory
{
	b2World* world;

	Chassis::Chassis(const std::string & filename)
	{
		std::cerr << "Loading " << filename << "...\n";
		std::ifstream fin(filename);
		YAML::Parser parser(fin);

		YAML::Node def;
		if (parser.GetNextDocument(def))
		{
			def["name"] >> name;
			def["size"] >> size;
			def["mass"] >> density;
			density /= size.x * size.y;
			def["turret_mount"] >> turret_mount;
			def["turret_speed"] >> turret_speed;
			def["tread_mount"] >> tread_mount;
			def["motor_mount"] >> motor_mount;
		}
	}

	Chassis::~Chassis() {}

	::Chassis* Chassis::produce(const b2v & pos) const
	{
		return new ::Chassis(world, pos, size, density, motor_mount, turret_mount, turret_speed, tread_mount);
	}

	Motor::Motor(const std::string & filename)
	{
		std::cerr << "Loading " << filename << "...\n";
		std::ifstream fin(filename);
		YAML::Parser parser(fin);

		YAML::Node def;
		if (parser.GetNextDocument(def))
		{
			def["name"] >> name;
			def["size"] >> size;
			def["mass"] >> density;
			density /= size.x * size.y;
			def["force"] >> max_force;
		}
	}

	Motor::~Motor() {}

	::Motor* Motor::produce(::Chassis & chassis) const
	{
		return new ::Motor(chassis, size, density, max_force);
	}

	Tread::Tread(const std::string & filename)
	{
		std::cerr << "Loading " << filename << "...\n";
		std::ifstream fin(filename);
		YAML::Parser parser(fin);

		YAML::Node def;
		if (parser.GetNextDocument(def))
		{
			def["name"] >> name;
			def["size"] >> size;
			def["mass"] >> density;
			density /= size.x * size.y;
		}
	}

	Tread::~Tread() {}

	::Tread* Tread::produce(const b2v & pos) const
	{
		return new ::Tread(world, size, pos, density);
	}

	Turret::Turret(const std::string & filename)
	{
		std::cerr << "Loading " << filename << "...\n";
		std::ifstream fin(filename);
		YAML::Parser parser(fin);

		YAML::Node def;
		if (parser.GetNextDocument(def))
		{
			def["name"] >> name;
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
}
