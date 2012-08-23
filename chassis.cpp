#include "engine.hpp"

Chassis::Chassis(const sf::Texture & texture, const b2v origin, const std::vector<b2FixtureDef*> & fixtures, b2World* world, const b2v & pos, float dir, float mo_mount, const b2v & tu_mount, float tu_speed, float tr_mount)
	: Entity(texture, origin, fixtures, world, pos, dir)
{
	motor_mount = mo_mount;
	turret_mount = tu_mount;
	tread_mount = tr_mount;
	turret_speed = tu_speed;
}

void Chassis::SetUserData(void* ptr)
{
	body->SetUserData(ptr);
}

void Chassis::update()
{
	sprite.setPosition(b2v2v2f(body->GetPosition()));
	sprite.setRotation(rad2deg(body->GetAngle()));
}
