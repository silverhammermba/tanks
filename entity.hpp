#ifndef ENTITY_H_
#define ENTITY_H_

class Entity
{
protected:
	sf::Sprite sprite;
	b2Body* body;
public:
	Entity(const sf::Texture & texture, const b2v origin, const std::vector<b2FixtureDef*> & fixtures, b2World* world, const b2v pos, float dir);
	~Entity();

	virtual entity_t type() const = 0;
	virtual void startContact() = 0;

	virtual void update();
	void draw_on(sf::RenderWindow & window) const;
};

#endif
