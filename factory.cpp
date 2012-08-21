#include "engine.hpp"

void operator >> (const YAML::Node& node, b2v & v)
{
	node[0] >> v.x;
	node[1] >> v.y;
}

namespace Factory
{
	b2World* world;

	Factory::Factory(const std::string & filename)
	{
		std::string texture_name(filename);
		texture_name.replace(texture_name.size() - 4, 4, "png").replace(0, 4, "graphics");
		std::cerr << "Loading " << texture_name << "...\n";

		texture.loadFromFile(texture_name);
	}

	Factory::~Factory() {}

	Chassis::Chassis(const std::string & filename) : Factory(filename)
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
		return new ::Chassis(texture, world, pos, size, density, motor_mount, turret_mount, turret_speed, tread_mount);
	}

	Motor::Motor(const std::string & filename) : Factory(filename)
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

	Tread::Tread(const std::string & filename) : Factory(filename)
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

	Turret::Turret(const std::string & filename) : Factory(filename)
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
			def["origin"] >> origin;
			def["gun_size"] >> gun_size;
			def["gun_mass"] >> gun_density;
			gun_density /= size.x * size.y;
			def["gun_origin"] >> gun_offset;
			def["impulse"] >> impulse;
			def["shot_size"] >> shot_size;
		}
	}

	Turret::~Turret() {}

	::Turret* Turret::produce(const b2v & pos) const
	{
		return new ::Turret(texture, world, pos, size, gun_size, gun_offset, density, gun_density, impulse, shot_size);
	}
}
