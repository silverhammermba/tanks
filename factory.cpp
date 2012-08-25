#include "engine.hpp"

void operator >> (const YAML::Node& node, b2v & v)
{
	node[0] >> v.x;
	node[1] >> v.y;
}

void operator >> (const YAML::Node& node, b2FixtureDef* fixture)
{
	const YAML::Node & verts = node["vertices"];
	b2v vertices[verts.size()];
	for (int i = 0; i < verts.size(); i++)
	{
		verts[i] >> vertices[i];
	}
	// TODO FREE THIS!
	b2PolygonShape* polygon = new b2PolygonShape;
	// pretty sure we don't need to keep vertices around
	polygon->Set(vertices, verts.size());
	fixture->shape = polygon;
	node["density"] >> fixture->density;
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

		std::cerr << "Loading " << filename << "...\n";
		std::ifstream fin(filename);
		YAML::Parser parser(fin);

		YAML::Node def;
		if (parser.GetNextDocument(def))
		{
			def["name"] >> name;
			def["origin"] >> origin;
			const YAML::Node & fixs = def["fixtures"];
			for (int i = 0; i < fixs.size(); i++)
			{
				b2FixtureDef* fixture = new b2FixtureDef;
				fixs[i] >> fixture;
				fixtures.push_back(fixture);
			}
		}
		// TODO raise error on else
	}

	Factory::~Factory()
	{
		for (auto fixt = fixtures.begin(); fixt != fixtures.end(); fixt++)
		{
			delete (*fixt)->shape;
			delete *fixt;
		}
	}

	Chassis::Chassis(const std::string & filename) : Factory(filename)
	{
		std::ifstream fin(filename);
		YAML::Parser parser(fin);

		YAML::Node def;
		if (parser.GetNextDocument(def))
		{
			def["turret_mount"] >> turret_mount;
			def["turret_speed"] >> turret_speed;
			def["tread_mount"] >> tread_mount;
			def["motor_mount"] >> motor_mount;
		}

		// common fixture settings
		for (auto fixt = fixtures.begin(); fixt != fixtures.end(); fixt++)
		{
			(*fixt)->friction = 0.3f;
			(*fixt)->filter.categoryBits = CATEGORY_TANK;
			(*fixt)->filter.maskBits     = CATEGORY_TANK | CATEGORY_SHOT | CATEGORY_WALL | CATEGORY_SMOKE;
		}
	}

	::Chassis* Chassis::produce(const b2v & pos, float dir) const
	{
		return new ::Chassis(texture, origin, fixtures, world, pos, dir, motor_mount, turret_mount, turret_speed, tread_mount);
	}

	Motor::Motor(const std::string & filename) : Factory(filename)
	{
		std::ifstream fin(filename);
		YAML::Parser parser(fin);

		YAML::Node def;
		if (parser.GetNextDocument(def))
		{
			def["force"] >> max_force;
		}

		// common fixture settings
		for (auto fixt = fixtures.begin(); fixt != fixtures.end(); fixt++)
		{
			(*fixt)->friction = 0.3f;
			(*fixt)->filter.categoryBits = CATEGORY_TANK;
			(*fixt)->filter.maskBits     = CATEGORY_TANK | CATEGORY_SHOT | CATEGORY_WALL | CATEGORY_SMOKE;
		}
	}

	::Motor* Motor::produce(const b2v & pos, float dir) const
	{
		return new ::Motor(texture, origin, fixtures, world, pos, dir, max_force);
	}

	Tread::Tread(const std::string & filename) : Factory(filename)
	{
		// common fixture settings
		for (auto fixt = fixtures.begin(); fixt != fixtures.end(); fixt++)
		{
			(*fixt)->friction = 0.3f;
			(*fixt)->filter.categoryBits = CATEGORY_TANK;
			(*fixt)->filter.maskBits     = CATEGORY_TANK | CATEGORY_SHOT | CATEGORY_WALL;
		}
	}


	::Tread* Tread::produce(const b2v & pos, float dir) const
	{
		return new ::Tread(texture, origin, fixtures, world, pos, dir);
	}

	Turret::Turret(const std::string & filename) : Factory(filename)
	{
		std::ifstream fin(filename);
		YAML::Parser parser(fin);

		YAML::Node def;
		if (parser.GetNextDocument(def))
		{
			def["impulse"] >> impulse;
		}

		// common fixture settings
		for (auto fixt = fixtures.begin(); fixt != fixtures.end(); fixt++)
		{
			(*fixt)->friction = 0.3f;
			(*fixt)->filter.categoryBits = CATEGORY_TURRET;
			// TODO how to set mask for barrel... rely on tunneling?
			(*fixt)->filter.maskBits     = CATEGORY_TURRET | CATEGORY_SHOT | CATEGORY_WALL | CATEGORY_SMOKE;
		}
	}

	::Turret* Turret::produce(const b2v & pos, float dir) const
	{
		return new ::Turret(texture, origin, fixtures, world, pos, dir, impulse);
	}

	Projectile::Projectile(const std::string & filename) : Factory(filename)
	{
		std::ifstream fin(filename);
		YAML::Parser parser(fin);

		YAML::Node def;
		if (parser.GetNextDocument(def))
		{
			def["damage"] >> damage;
		}

		// common fixture settings
		for (auto fixt = fixtures.begin(); fixt != fixtures.end(); fixt++)
		{
			(*fixt)->friction = 0.8f;
			(*fixt)->filter.categoryBits = CATEGORY_SHOT;
			(*fixt)->filter.maskBits     = CATEGORY_TANK | CATEGORY_WALL | CATEGORY_TURRET;
		}
	}

	::Projectile* Projectile::produce(const b2v & pos, float dir, Tank* owner, float impulse) const
	{
		return new ::Projectile(texture, origin, fixtures, world, pos, dir, owner, impulse);
	}

	// TODO hmmmmm
	::Wall* Wall::produce(b2v size, const b2v & pos, float dir) const
	{
	}
}
