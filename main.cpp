#include <iostream>
#include <sstream>
#include <cmath>
#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>
#include "helpers.hpp"

#define _USE_MATH_DEFINES

using std::cin;
using std::cerr;
using std::endl;

void move_origin(sf::RectangleShape & tank, float pos);

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

	sf::RenderWindow window
	{
		sf::VideoMode(800, 600, 32),
		"Light Cycles",
		sf::Style::Titlebar
	};
	window.setVerticalSyncEnabled(false);

	sf::View view (window.getView());

	sf::Event event;

	sf::RectangleShape tank {v2f {30, 30}};
	tank.setOrigin(15, 15);
	tank.setPosition(view.getCenter());
	tank.setFillColor(sf::Color(255, 0, 0));

	sf::RectangleShape origin {v2f {1, 1}};
	origin.setOrigin(0.5f, 0.5f);
	origin.setFillColor(sf::Color(0, 0, 255));

	while (window.isOpen())
	{
		// process input
		while (window.pollEvent(event))
		{
			if (event.type == sf::Event::Closed)
				window.close();
			else if (event.type == sf::Event::Resized)
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
			// quit
			if (event.type == sf::Event::KeyReleased && event.key.code == sf::Keyboard::Escape)
				window.close();
		}

		float ftime = fclock.getElapsedTime().asSeconds();
		fclock.restart();

		float time = clock.getElapsedTime().asSeconds();

		move_origin(tank, std::sin(time) * 15.f + 15.f);
		origin.setPosition(tank.getPosition());

		fps_s.str("");
		fps_s << "FPS " << int (1.f / ftime);
		fps.setString(fps_s.str());

		window.clear(sf::Color(255, 255, 255));

		window.draw(tank);
		window.draw(origin);
		window.draw(fps);

		window.display();
	}

	return EXIT_SUCCESS;
}

void move_origin(sf::RectangleShape & tank, float pos)
{
	float origin = tank.getOrigin().x;
	tank.setOrigin(v2f(pos, 15.f));
	tank.move(pos - origin, 0.f);
}
