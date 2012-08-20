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

	class Chassis
	{
		string name;
		b2v size;
		float density;
		b2v turret_mount;
		float turret_speed;
		float tread_mount;
		float motor_mount;
	public:
		Chassis(const std::string & filename);
		~Chassis();

		::Chassis* produce(const b2v & pos) const;
	};

	class Motor
	{
		string name;
		b2v size;
		float density;
		float max_force;
	public:
		Motor(const std::string & filename);
		~Motor();

		::Motor* produce(::Chassis & chassis) const;
	};

	class Tread
	{
		string name;
		b2v size;
		float max_force;
		float density;
	public:
		Tread(const std::string & filename);
		~Tread();

		::Tread* produce(const b2v & pos) const;
	};

	class Turret
	{
		string name;
		b2v size;
		b2v gun_size;
		float gun_offset;
		float density;
		float gun_density;
		float impulse;
		float shot_size;
	public:
		Turret(const std::string & filename);
		~Turret();

		::Turret* produce(const b2v & pos) const;
	};
}

#endif
