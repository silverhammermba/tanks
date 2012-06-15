#include <iostream>
#include "projectile.hpp"

Projectile::Projectile(Tank *own, const v2f pos, const v2f traj, float sp, float size)
	: shot(v2f(size, size)), trajectory(traj)
{
	//std::cerr << "Creating projectile at " << pos.x << "," << pos.y << "\n";
	owner = own;
	speed = sp;

	shot.setOrigin(shot.getSize() / 2.f);
	shot.setPosition(pos);
	shot.setFillColor(sf::Color(0.f, 0.f, 0.f));
}

Projectile::~Projectile()
{
	//std::cerr << "Deleting projectile\n";
}

void Projectile::draw_on(sf::RenderWindow & window) const
{
	window.draw(shot);
}

void Projectile::move(float time)
{
	shot.move(trajectory * time * speed);
}
