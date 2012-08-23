#ifndef FACTORY_H_
#define FACTORY_H_

namespace Factory
{
	using std::string;

	extern b2World* world;

	template <class T>
	int load_dir(std::string dir, std::list<T*> & list)
	{
		DIR* dp;
		struct dirent* dirp;
		if ((dp = opendir(dir.c_str())) == NULL)
		{
			std::cerr << "Error " << errno << " reading " << dir << "\n";
			exit;
		}

		std::cerr << "Loading " << dir << "/...\n";
		while ((dirp = readdir(dp)) != NULL)
		{
			std::string filename(dirp->d_name);

			if (filename[0] != '.')
				list.push_back(new T(dir + "/" + filename));
		}
		closedir(dp);
		return 0;
	}

	class Factory
	{
	protected:
		string name;
		sf::Texture texture;
		b2v origin;
		std::vector<b2FixtureDef*> fixtures;
	public:
		Factory(const std::string & filename);
		~Factory();
	};

	class Chassis : public Factory
	{
		b2v turret_mount;
		float turret_speed;
		float tread_mount;
		float motor_mount;
	public:
		Chassis(const std::string & filename);

		::Chassis* produce(const b2v & pos, float dir) const;
	};

	class Motor : public Factory
	{
		float max_force;
	public:
		Motor(const std::string & filename);

		::Motor* produce(const b2v & pos, float dir) const;
	};

	class Tread : public Factory
	{
	public:
		Tread(const std::string & filename);

		::Tread* produce(const b2v & pos, float dir) const;
	};

	class Turret : public Factory
	{
		float impulse;
	public:
		Turret(const std::string & filename);

		::Turret* produce(const b2v & pos, float dir) const;
	};

	class Projectile : public Factory
	{
		float damage;
	public:
		Projectile(const std::string & filename);
		~Projectile();

		::Projectile* produce(const b2v & pos, float dir, Tank* owner, float impulse) const;
	};

	class Wall
	{
	public:
		Wall() {};

		::Wall* produce(b2v size, const b2v & pos, float dir) const;
	};
}

#endif
