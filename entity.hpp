#ifndef ENTITY_H_
#define ENTITY_H_

#include "helpers.hpp"

class Entity
{
public:
	virtual entity_t type() const = 0;
	virtual void startContact() = 0;
};

#endif
