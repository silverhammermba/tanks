#include "factory.hpp"

namespace Factory
{
	Tread::Tread(const std::string & name)
	{
		YAML::Node def = YAML::LoadFile(name);

		size = def["size"].as<b2v>();
		max_force = def["force"].as<float>();
		density = def["weight"].as<float>() / (size.x * size.y);
	}

	::Tread* Tread::produce(const b2v & pos) const
	{
		return new ::Tread(world, size, pos, max_force, density);
	}

	Turret::Turret(const std::string & name)
	{
		YAML::Node def = YAML::LoadFile(name);

		size = def["size"].as<b2v>();
		gun_size = def["gun size"].as<b2v>();
		gun_offset = def["offset"].as<float>();
		density = def["weight"].as<float>() / (size.x * size.y);
		gun_density = def["gun weight"].as<float>() / (gun_size.x * gun_size.y);
		impulse = def["impulse"].as<float>();
		shot_size = def["shot size"].as<float>();
	}

	::Turret* Turret::produce(const b2v & pos) const
	{
		return new ::Turret(world, pos, size, gun_size, gun_offset, density, gun_density, impulse, shot_size);
	}

	Chassis::Chassis(const std::string & name)
	{
		YAML::Node def = YAML::LoadFile(name);

		size = def["size"].as<b2v>();
		density = def["weight"].as<float>() / (size.x * size.y);
	}

	::Chassis* Chassis::produce(const b2v & pos) const
	{
		sf::Color color(0, 0, 0);
		return new ::Chassis(world, pos, size, density, color);
	}
}
