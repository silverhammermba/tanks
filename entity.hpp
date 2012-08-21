#ifndef ENTITY_H_
#define ENTITY_H_

class Entity
{
public:
	virtual entity_t type() const = 0;
	virtual void startContact() = 0;

	virtual void update() = 0;
	virtual void draw_on(sf::RenderWindow & window) const = 0;
};

#endif
