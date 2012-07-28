#include <iostream>
#include <sstream>
#include <list>
#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>
#include <Box2D/Box2D.h>
#include "helpers.hpp"
#include "tank.hpp"
#include "projectile.hpp"

using std::cin;
using std::cerr;
using std::endl;

void set_up(sf::RenderWindow & window, sf::View & view, sf::FloatRect & screen);
sf::RectangleShape* add_wall(b2Body *ground, float width, float height, float x, float y);

int main(int argc, char *argv[])
{
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
	window.setFramerateLimit(0);

	sf::View view (window.getView());
	sf::FloatRect screen (0.f, 0.f, window.getSize().x, window.getSize().x);

	// this isn't flipping the screen properly
	set_up(window, view, screen);

	sf::Event event;

	std::list<Tank *> players;
	std::list<Projectile *> shots;

	/***** Box2D *****/
	b2v bodySize(10.f, 8.f);

	b2World world(b2Vec2(0.0f, 0.0f));

	// create ground box
	b2BodyDef groundBody;
	groundBody.position.Set(0.0f, 0.0f);
	b2Body* ground = world.CreateBody(&groundBody);

	b2PolygonShape groundBox;
	groundBox.SetAsBox(50.f, 50.f);
	b2FixtureDef groundFixture;
	groundFixture.shape = &groundBox;
	// doesn't collide with anything
	groundFixture.filter.categoryBits = 0x0000;
	groundFixture.filter.maskBits     = 0x0000;

	ground->CreateFixture(&groundFixture);
	sf::RectangleShape groundRect(v2f(100.f * ppm, 100.f * ppm));
	groundRect.setOrigin(50.f * ppm, 50.f * ppm);
	groundRect.setFillColor(sf::Color(40, 40, 40));
	b2Vec2 groundPos = ground->GetPosition();
	groundRect.setPosition(b2v2v2f(groundPos));

	// walls
	std::list<sf::RectangleShape *> walls;
	walls.push_back(add_wall(ground, 10.f, 250.f, -100.f, 0.f));
	walls.push_back(add_wall(ground, 10.f, 250.f, 100.f, 0.f));
	walls.push_back(add_wall(ground, 250.f, 10.f, 0.f, -100.f));
	walls.push_back(add_wall(ground, 250.f, 10.f, 0.f, 100.f));

	// set up simulation
	float timeStep = 1.0f / 60.0f;

	int velocityIterations = 6;
	int positionIterations = 2;

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
					players.push_back(new Tank(event.joystickButton.joystickId, &world, ground, bodySize, b2v(0, 0), sf::Color(rand() % 256, rand() % 256, rand() % 256)));
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
			/*
			if ((*player)->is_firing())
				shots.push_back((*player)->fire());
			*/
		}

		float ptime = pclock.getElapsedTime().asSeconds();
		if (ptime >= timeStep)
		{
			pclock.restart();
			world.Step(ptime * timescale, velocityIterations, positionIterations);

			for (auto player = players.begin(); player != players.end(); player++)
			{
				(*player)->update();
			}
		}

		/*
		for (auto shot = shots.begin(); shot != shots.end(); shot++)
			(*shot)->move(ftime);
		*/

		fps_s.str("");
		fps_s << "FPS " << int (1.f / ftime);
		fps_s << "\nPHY " << int (1.f / ptime);
		fps.setString(fps_s.str());

		window.clear(sf::Color(255, 255, 255));
		window.draw(groundRect);
		
		for (auto wall = walls.begin(); wall != walls.end(); wall++)
			window.draw(**wall);

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

		for (auto player = players.begin(); player != players.end(); player++)
			(*player)->draw_on(window);

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
	screen = sf::FloatRect(topleft.x, topleft.y, botright.x - topleft.x, botright.y - topleft.y);
}

sf::RectangleShape* add_wall(b2Body *ground, float width, float height, float x, float y)
{
	b2PolygonShape box;
	b2Vec2 size(width, height);
	b2Vec2 pos(x, y);
	box.SetAsBox(width / 2.f, height / 2.f, pos, 0);
	b2FixtureDef fixture;
	fixture.shape = &box;
	fixture.filter.categoryBits = 0x0001;
	fixture.filter.maskBits     = 0x0001;

	ground->CreateFixture(&fixture);
	b2Vec2 groundPos = ground->GetPosition();

	sf::RectangleShape* rect = new sf::RectangleShape(b2v2v2f(size));
	rect->setOrigin(b2v2v2f(size) / 2.f);
	rect->setFillColor(sf::Color(100, 100, 100));
	rect->setPosition(b2v2v2f(groundPos + pos));

	return rect;
}
