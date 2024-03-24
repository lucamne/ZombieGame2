#pragma once

#include "Agent.h"

class Human : public Agent
{
public:
	Human();
	virtual ~Human();

	void init(float speed, glm::vec2 pos);

	virtual void update(const std::vector<std::string>& level_data,
		std::vector<Human*> humans,
		std::vector<Zombie*> zombies);

private:
	glm::vec2 m_direction{};
};

