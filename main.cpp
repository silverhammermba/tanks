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

	float width = 30.f;
	float speed = 2.f;

	sf::RectangleShape tank {v2f {width, width}};
	tank.setOrigin(width / 2.f, width / 2.f);
	tank.setPosition(view.getCenter());
	tank.setFillColor(sf::Color(255, 0, 0));

	sf::RectangleShape origin {v2f {1, 1}};
	origin.setOrigin(0.5f, 0.5f);
	origin.setFillColor(sf::Color(0, 0, 255));

	sf::RectangleShape direction {v2f {60, 1}};
	direction.setOrigin(0.f, 0.5f);
	direction.setFillColor(sf::Color(0, 255, 0));

	set_up(window, view);
	int joystick = 0;
	float left, right;

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
			else if (event.type == sf::Event::JoystickButtonPressed && event.joystickButton.button == 7)
				joystick = event.joystickButton.joystickId;
			/*
			else if (event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::Left)
				move_origin(tank, 0);
			else if (event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::Up)
				move_origin(tank, 15);
			else if (event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::Right)
				move_origin(tank, 30);
			*/
		}

		float ftime = fclock.getElapsedTime().asSeconds();
		fclock.restart();

		float time = clock.getElapsedTime().asSeconds();

		if (sf::Joystick::isConnected(joystick))
		{
			left = -sf::Joystick::getAxisPosition(joystick, sf::Joystick::Axis::Y);
			right = -sf::Joystick::getAxisPosition(joystick, sf::Joystick::Axis::V);
			// TODO improve deadzone calculation
			if (std::abs(left) < 30.f)
				left = 0;
			if (std::abs(right) < 30.f)
				right = 0;

			if (left == right)
			{
				move_origin(tank, width / 2.f);
				// TODO refactor
				tank.move(v2f(std::cos(tank.getRotation()*M_PI/180.f), std::sin(tank.getRotation()*M_PI/180)) * (speed * width * left * ftime * 3.1415926539f / 180.f));
			}
			else
			{
				move_origin(tank, (width * left) / (left - right));
				tank.rotate(ftime * (left - right) * speed);
			}
		}

		//move_origin(tank, std::sin(time) * 60.f + 15.f);
		origin.setPosition(tank.getPosition());

		direction.setPosition(tank.getPosition());
		direction.setRotation(tank.getRotation());

		fps_s.str("");
		fps_s << "FPS " << int (1.f / ftime);
		fps.setString(fps_s.str());

		window.clear(sf::Color(255, 255, 255));

		window.draw(tank);
		window.draw(direction);
		window.draw(origin);
		window.draw(fps);

		// TODO refactor, could be useful
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

		window.display();
	}

	return EXIT_SUCCESS;
}

void move_origin(sf::RectangleShape & tank, float pos)
{
	sf::FloatRect before = tank.getGlobalBounds();
	tank.setOrigin(v2f(15.f, pos));
	sf::FloatRect after = tank.getGlobalBounds();
	tank.move(before.left - after.left, before.top - after.top);
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
