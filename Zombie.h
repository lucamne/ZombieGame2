#pragma once

#include "Agent.h"

class Zombie : public Agent
{
public:
	Zombie();
	~Zombie();

	virtual void update(const std::vector<std::string>& level_data,
		std::vector<Human*> humans,
		std::vector<Zombie*> zombies);
};

