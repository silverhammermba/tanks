#include "engine.hpp"

void operator >> (const YAML::Node& node, b2v & v)
{
	node[0] >> v.x;
	node[1] >> v.y;
}

void operator >> (const YAML::Node& node, b2FixtureDef* fixture)
{
	node["density"] >> fixture->density;

	bool symmetry;
	if (const YAML::Node* pkey = node.FindValue("symmetry"))
		*pkey >> symmetry;
	else
		symmetry = false;

	const YAML::Node & verts = node["vertices"];
	int number = verts.size() * (symmetry ? 2 : 1);
	b2v vertices[number];
	for (int i = 0; i < verts.size(); i++)
	{
		verts[i] >> vertices[i];
	}
	if (symmetry)
	{
		for (int i = verts.size(), j = verts.size() - 1; j >= 0; i++, j--)
		{
			if (vertices[j].y != 0.f)
				vertices[i] = b2v(vertices[j].x, -vertices[j].y);
		}
	}

	b2PolygonShape* polygon = new b2PolygonShape;
	// pretty sure we don't need to keep vertices around
	polygon->Set(vertices, number);
	fixture->shape = polygon;
}

namespace Factory
{
	b2World* world;

	Factory::Factory(const std::string & filename)
	{
		std::string texture_name(filename);
		texture_name.replace(texture_name.size() - 4, 4, "png").replace(0, 4, "graphics");
		std::cerr << "Loading " << texture_name << "...\n";
		// TODO raise exception if file does not exist
		texture.loadFromFile(texture_name);

		std::cerr << "Loading " << filename << "...\n";
		std::ifstream fin(filename);
		YAML::Parser parser(fin);

		YAML::Node def;
		parser.GetNextDocument(def);

		if (const YAML::Node* pkey = def.FindValue("name"))
			*pkey >> name;
		else // TODO need better placeholder name
			name = std::string(filename);

		if (const YAML::Node* pkey = def.FindValue("origin"))
			*pkey >> origin;
		else
			origin = b2v(0, 0);

		const YAML::Node & fixs = def["fixtures"];
		for (int i = 0; i < fixs.size(); i++)
		{
			b2FixtureDef* fixture = new b2FixtureDef;
			fixs[i] >> fixture;
			fixtures.push_back(fixture);
		}
		// TODO reflect fixtures
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
	}

	::Chassis* Chassis::produce(int player, const b2v & pos, float dir) const
	{
		// common fixture settings
		for (auto fixt = fixtures.begin(); fixt != fixtures.end(); fixt++)
		{
			(*fixt)->friction = 0.3f;
			(*fixt)->filter.categoryBits = CATEGORY_CHASSIS(player);
			(*fixt)->filter.maskBits     = CATEGORY_CHASSES | CATEGORY_SHOT | CATEGORY_WALL | CATEGORY_SMOKE;
		}
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
	}

	::Motor* Motor::produce(int player, const b2v & pos, float dir) const
	{
		// common fixture settings
		for (auto fixt = fixtures.begin(); fixt != fixtures.end(); fixt++)
		{
			(*fixt)->friction = 0.3f;
			(*fixt)->filter.categoryBits = CATEGORY_CHASSIS(player);
			(*fixt)->filter.maskBits     = CATEGORY_CHASSES | CATEGORY_SHOT | CATEGORY_WALL | CATEGORY_SMOKE;
		}
		return new ::Motor(texture, origin, fixtures, world, pos, dir, max_force);
	}

	Tread::Tread(const std::string & filename) : Factory(filename)
	{
		// TODO load breaking power
		// TODO load right tread differently? so origin is against tank
	}


	::Tread* Tread::produce(int player, const b2v & pos, float dir) const
	{
		// common fixture settings
		for (auto fixt = fixtures.begin(); fixt != fixtures.end(); fixt++)
		{
			(*fixt)->friction = 0.3f;
			(*fixt)->filter.categoryBits = CATEGORY_CHASSIS(player);
			(*fixt)->filter.maskBits     = CATEGORY_CHASSES | CATEGORY_SHOT | CATEGORY_WALL;
		}
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
	}

	::Turret* Turret::produce(int player, const b2v & pos, float dir) const
	{
		// common fixture settings
		for (auto fixt = fixtures.begin(); fixt != fixtures.end(); fixt++)
		{
			(*fixt)->friction = 0.3f;
			(*fixt)->filter.categoryBits = CATEGORY_TURRET(player);
			(*fixt)->filter.maskBits     = CATEGORY_TURRETS | CATEGORY_SHOT | CATEGORY_WALL | CATEGORY_SMOKE;
		}
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
		}
		mask = CATEGORY_WALL | CATEGORY_CHASSES | CATEGORY_TURRETS;
	}

	::Projectile* Projectile::produce(int player, const b2v & pos, float dir, Tank* owner, float impulse) const
	{
		// TODO too expensive?
		for (auto fixt = fixtures.begin(); fixt != fixtures.end(); fixt++)
			(*fixt)->filter.maskBits = mask ^ CATEGORY_CHASSIS(player) ^ CATEGORY_TURRET(player);

		return new ::Projectile(texture, origin, fixtures, world, pos, dir, owner, impulse);
	}
}
