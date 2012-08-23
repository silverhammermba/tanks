#include "engine.hpp"

class ShotListener : public b2ContactListener
{
public:
	void BeginContact(b2Contact* contact)
	{
		void* ent = contact->GetFixtureA()->GetBody()->GetUserData();
		if (ent)
			static_cast<Entity*>(ent)->startContact();

		ent = contact->GetFixtureB()->GetBody()->GetUserData();
		if (ent)
			static_cast<Entity*>(ent)->startContact();
	}
};

void set_up(sf::RenderWindow & window, sf::View & view, sf::FloatRect & screen);

int main(int argc, char *argv[])
{
	srand((unsigned int)time(NULL));
	sf::Clock fclock; // frame clock
	sf::Clock pclock; // physics clock
	sf::Clock clock; // accumulative clock
	float timescale = 1.f;

	sf::Text fps;
	fps.setScale(1.f, -1.f);
	fps.setCharacterSize(12);
	fps.setPosition(5.f, 5.f);
	fps.setColor(sf::Color(0, 0, 0));
	std::ostringstream fps_s;

	sf::Text dbg;
	dbg.setCharacterSize(12);
	dbg.setColor(sf::Color(0, 0, 0));
	std::ostringstream dbg_s;

	sf::RenderWindow window
	{
		sf::VideoMode(800, 600, 32),
		"Tank Battle"
	};
	window.setVerticalSyncEnabled(false);
	// TODO figure out how to divorce graphics and physics
	window.setFramerateLimit(120);

	sf::View view (window.getView());
	sf::FloatRect screen (0.f, 0.f, window.getSize().x, window.getSize().x);

	// this isn't flipping the screen properly
	set_up(window, view, screen);

	sf::Event event;

	// lists for storing game objects
	std::list<Tank*> players;
	std::list<Projectile*> shots;
	std::list<Particle*> smoke;

	b2World world(b2Vec2(0.0f, 0.0f));

	// factories for creating tanks
	Factory::world = &world;
	
	std::list<Factory::Chassis*> chasses;
	std::list<Factory::Motor*> motors;
	std::list<Factory::Tread*> treads;
	std::list<Factory::Turret*> turrets;
	std::list<Factory::Projectile*> projectiles;

	Factory::load_dir("yaml/chasses", chasses);
	Factory::load_dir("yaml/motors", motors);
	Factory::load_dir("yaml/treads", treads);
	Factory::load_dir("yaml/turrets", turrets);
	Factory::load_dir("yaml/projectiles", projectiles);

	ShotListener listener;
	world.SetContactListener(&listener);

	// create ground box
	b2BodyDef groundBody;
	groundBody.position.Set(0.0f, 0.0f);
	b2Body* ground = world.CreateBody(&groundBody);

	b2v groundSize(100.f, 100.f);
	b2PolygonShape groundBox;
	groundBox.SetAsBox(groundSize.x / 2.f, groundSize.y / 2.f);
	b2FixtureDef groundFixture;
	groundFixture.shape = &groundBox;
	// doesn't collide with anything
	groundFixture.filter.categoryBits = CATEGORY_GROUND;
	groundFixture.filter.maskBits     = 0;

	ground->CreateFixture(&groundFixture);
	sf::RectangleShape groundRect(b2v2v2f(groundSize));
	groundRect.setOrigin(b2v2v2f(groundSize) / 2.f);
	groundRect.setFillColor(sf::Color(150, 150, 150));
	b2Vec2 groundPos = ground->GetPosition();
	groundRect.setPosition(b2v2v2f(groundPos));

	// walls
	b2BodyDef wallBody;
	wallBody.position.Set(0.0f, 0.0f);
	Wall::body = world.CreateBody(&wallBody);

	std::list<Wall *> walls;
	walls.push_back(new Wall(b2v(3.f, 100.f), b2v(-48.5f, 0.f)));
	walls.push_back(new Wall(b2v(3.f, 100.f), b2v(48.5f, 0.f)));
	walls.push_back(new Wall(b2v(100.f, 3.f), b2v(0.f, -48.5f)));
	walls.push_back(new Wall(b2v(100.f, 3.f), b2v(0.f, 48.5f)));

	// set up simulation
	float timeStep = 1.0f / 60.0f;

	int velocityIterations = 6;
	int positionIterations = 2;

	// TODO testing
	//players.push_back(new Tank(0 , &world, b2v(0, 0), chassis, turret, tread));

	while (window.isOpen())
	{
		// process input
		while (window.pollEvent(event))
		{
			// quit
			if (event.type == sf::Event::Closed)
				window.close();
			else if (event.type == sf::Event::KeyReleased && event.key.code == sf::Keyboard::Escape)
				window.close();
			else if (event.type == sf::Event::Resized)
				set_up(window, view, screen);
			// remove player
			else if (event.type == sf::Event::JoystickButtonPressed && event.joystickButton.button == 6)
			{
				Tank *tank = NULL;
				for (auto player = players.begin(); player != players.end(); player++)
				{
					if ((*player)->get_joystick() == event.joystickButton.joystickId)
					{
						tank = *player;
						break;
					}
				}
				if (tank != NULL)
				{
					delete tank;
					players.remove(tank);
				}
			}
			// add player
			else if (event.type == sf::Event::JoystickButtonPressed && event.joystickButton.button == 7)
			{
				bool taken = false;
				for (auto player = players.begin(); player != players.end(); player++)
				{
					if ((*player)->get_joystick() == event.joystickButton.joystickId)
					{
						taken = true;
						break;
					}
				}
				if (!taken)
					players.push_back(new Tank(event.joystickButton.joystickId, &world, b2v(0, 0), *chasses.front(), *motors.front(), *turrets.front(), *treads.front(), *projectiles.front()));
			}
			else
			{
				for (auto player = players.begin(); player != players.end(); player++)
					(*player)->bind(event);
			}
		}

		float ftime = fclock.getElapsedTime().asSeconds();
		fclock.restart();

		float time = clock.getElapsedTime().asSeconds();

		for (auto player = players.begin(); player != players.end(); player++)
		{
			(*player)->read_controller();
			(*player)->move();
			if ((*player)->is_firing())
				shots.push_back((*player)->fire());
		}

		float ptime = pclock.getElapsedTime().asSeconds();
		if (ptime >= timeStep)
		{
			for (auto shot = shots.begin(); shot != shots.end(); shot++)
			{
				if ((*shot)->should_explode)
				{
					b2v pos = (*shot)->pos();
					b2v norm = (*shot)->fwd_norm();
					for (int i = 0; i < rand_i(100); i++)
						smoke.push_back(new Particle(&world, pos, norm, CATEGORY_TANK));
					for (int i = 0; i < rand_i(60); i++)
						smoke.push_back(new Particle(&world, pos, norm, CATEGORY_TURRET));
					for (int i = 0; i < rand_i(40); i++)
						smoke.push_back(new Particle(&world, pos, norm, 0));

					(*shot)->explode();
					delete *shot;
					shot = shots.erase(shot);
				}
			}

			pclock.restart();
			world.Step(ptime * timescale, velocityIterations, positionIterations);

			for (auto player = players.begin(); player != players.end(); player++)
				(*player)->update();
			for (auto shot = shots.begin(); shot != shots.end(); shot++)
				(*shot)->update();
			for (auto part = smoke.begin(); part != smoke.end(); part++)
				(*part)->update(ptime * timescale);

		}

		fps_s.str("");
		fps_s << "FPS " << int (1.f / ftime);
		fps_s << "\nPHY " << int (1.f / ptime);
		fps.setString(fps_s.str());

		window.clear(sf::Color(255, 255, 255));
		window.draw(groundRect);
		
		for (auto wall = walls.begin(); wall != walls.end(); wall++)
			(*wall)->draw_on(window);

		for (auto player = players.begin(); player != players.end(); player++)
			(*player)->draw_on(window);

		for (auto player = players.begin(); player != players.end(); player++)
			(*player)->get_turret()->draw_on(window);

		for (auto shot = shots.begin(); shot != shots.end(); shot++)
		{
			if (screen.intersects((*shot)->getGlobalBounds()))
				(*shot)->draw_on(window);
			else
			{
				delete *shot;
				shot = shots.erase(shot);
			}
		}

		for (auto part = smoke.begin(); part != smoke.end(); part++)
		{
			if ((*part)->expired())
			{
				delete *part;
				part = smoke.erase(part);
			}
			else
				(*part)->draw_on(window);
		}

		window.draw(fps);

		window.display();
	}

	return EXIT_SUCCESS;
}

void set_up(sf::RenderWindow & window, sf::View & view, sf::FloatRect & screen)
{
	sf::Vector2u size = window.getSize();
	float prop = float (size.x) / size.y;
	if (size.x * 3 < size.y * 4)
	{
		view.setSize(v2f(800, -(800.f * size.y) / size.x));
	}
	else if (size.x * 3 > size.y * 4)
	{
		view.setSize(v2f((600.f * size.x) / size.y, -600.f));
	}
	view.setCenter(0.f, 0.f);
	window.setView(view);
	v2f topleft = window.convertCoords(sf::Vector2i(0, 0));
	v2f botright = window.convertCoords(sf::Vector2i(window.getSize().x, window.getSize().y));
	screen = sf::FloatRect(topleft.x, -topleft.y, botright.x - topleft.x, topleft.y - botright.y);
}
