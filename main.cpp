#include <iostream>
#include <sstream>
#include <list>
#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>
#include "helpers.hpp"
#include "tank.hpp"
#include "projectile.hpp"

using std::cin;
using std::cerr;
using std::endl;

void move_origin(sf::RectangleShape & tank, float pos);
void set_up(sf::RenderWindow & window, sf::View & view);

int main(int argc, char *argv[])
{
	sf::Clock fclock; // frame fclock
	sf::Clock clock; // accumulative clock
	float timescale = 1.f;

	sf::Text fps;
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
	window.setFramerateLimit(120);

	sf::View view (window.getView());

	sf::Event event;

	std::list<Tank *> players;
	std::list<Projectile *> shots;

	set_up(window, view);

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
				set_up(window, view);
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
					players.push_back(new Tank(event.joystickButton.joystickId, v2f(30.f, 35.f), v2f(view.getCenter()), sf::Color(rand() % 256, rand() % 256, rand() % 256)));
			}
		}

		float ftime = fclock.getElapsedTime().asSeconds();
		fclock.restart();

		float time = clock.getElapsedTime().asSeconds();

		for (auto player = players.begin(); player != players.end(); player++)
		{
			(*player)->read_controller();
			(*player)->move(ftime);
		}

		fps_s.str("");
		fps_s << "FPS " << int (1.f / ftime);
		fps.setString(fps_s.str());

		window.clear(sf::Color(255, 255, 255));

		for (auto player = players.begin(); player != players.end(); player++)
			(*player)->draw_on(window);
		window.draw(fps);

		// TODO refactor, could be useful
		/*
		dbg_s.str("");
		dbg_s << "Left: " << left;
		dbg.setString(dbg_s.str());
		dbg.setPosition(5.f, 30.f);
		window.draw(dbg);
		dbg_s.str("");
		dbg_s << "Right: " << right;
		dbg.setString(dbg_s.str());
		dbg.setPosition(5.f, 40.f);
		window.draw(dbg);
		dbg_s.str("");
		dbg_s << "Theta: " << tank.getRotation();
		dbg.setString(dbg_s.str());
		dbg.setPosition(5.f, 50.f);
		window.draw(dbg);
		*/

		window.display();
	}

	return EXIT_SUCCESS;
}

void set_up(sf::RenderWindow & window, sf::View & view)
{
	sf::Vector2u size = window.getSize();
	float prop = float (size.x) / size.y;
	if (size.x * 3 < size.y * 4)
	{
		view.setSize(v2f(800, (800.f * size.y) / size.x));
		window.setView(view);
	}
	else if (size.x * 3 > size.y * 4)
	{
		view.setSize(v2f((600.f * size.x) / size.y, 600.f));
		window.setView(view);
	}
}
