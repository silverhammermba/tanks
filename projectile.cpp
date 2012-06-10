#include "projectile.hpp"

Projectile::Projectile(Tank *own, const v2f pos, const v2f traj)
	: shot(pos), trajectory(traj)
{
	owner = own;
}

Projectile::~Projectile()
{
}

void Projectile::draw_on(sf::RenderWindow & window) const
{
	window.draw(shot);
}

void Projectile::move(float time)
{
	shot.move(trajectory * time);
}
