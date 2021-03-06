#ifndef WALL_H_
#define WALL_H_

class Wall : public Entity
{
	sf::RectangleShape rect;
public:
	static b2Body* body;
	Wall(const b2v & size, const b2v & pos, float angle = 0.f);
	~Wall() {};

	entity_t type() const { return WALL; };
	void startContact() {};
	void update() {};
	void draw_on(sf::RenderWindow & window) const;
};

#endif
