#pragma once

#include "Agent.h"

class Human : public Agent
{
public:
	Human();
	virtual ~Human();

	virtual void update(const std::vector<std::string>& level_data,
		std::vector<Human*> humans,
		std::vector<Zombie*> zombies);
};

