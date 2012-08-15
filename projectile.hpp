#ifndef PROJECTILE_H_
#define PROJECTILE_H_

class Tank;

class Projectile : public Entity
{
	Tank *owner;

	b2Body* body;
	sf::RectangleShape rect;

	b2v velocity;
public:
	bool should_explode;

	Projectile(b2World* world, Tank *own, const b2v pos, float dir, float sp, b2v size);
	~Projectile();

	entity_t type() const { return SHOT; };
	b2v pos() const { return body->GetPosition(); };
	b2v fwd_norm() const { return body->GetWorldVector(b2v(1.f, 0.f)); };
	void startContact();
	void explode();
	void update();
	sf::FloatRect getGlobalBounds() const { return rect.getGlobalBounds(); };
	void draw_on(sf::RenderWindow & window) const;
};

#endif
