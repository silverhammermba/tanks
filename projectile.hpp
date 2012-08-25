#ifndef PROJECTILE_H_
#define PROJECTILE_H_

class Tank;

class Projectile : public Entity
{
	Tank *owner;
public:
	bool should_explode;

	Projectile(const sf::Texture & texture, const b2v & origin, const std::vector<b2FixtureDef*> fixtures, b2World* world, const b2v pos, float dir, Tank *own, float imp);

	entity_t type() const { return SHOT; };
	b2v pos() const { return body->GetPosition(); };
	b2v fwd_norm() const { return body->GetWorldVector(b2v(1.f, 0.f)); };
	void startContact();
	void explode();
	sf::FloatRect getGlobalBounds() const { return sprite.getGlobalBounds(); };
};

#endif
