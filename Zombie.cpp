#include "Zombie.h"
#include "Human.h"

Zombie::Zombie()
{
}

Zombie::~Zombie()
{
}

void Zombie::init(float speed, glm::vec2 pos)
{
	m_health = 150.0f;
	m_speed = speed;
	m_position = pos;
	m_color = { 0,160,0,255 };
}

void Zombie::update(const std::vector<std::string>& level_data,
	std::vector<Human*>& humans,
	[[maybe_unused]] std::vector<Zombie*>& zombies)
{
	Human* closest_human{ getNearestHuman(humans) };

	if (closest_human != nullptr)
	{
		glm::vec2 direction{ glm::normalize(closest_human->getPosition() - m_position) };
		m_position += direction * m_speed;
	}

	collideWithLevel(level_data);
}

Human* Zombie::getNearestHuman(const std::vector<Human*>& humans)
{
	Human* closest_human{ nullptr };
	float smallest_distance{ 999999999.0f };

	for (int i{ 0 }; i < humans.size(); i++)
	{
		glm::vec2 dist_vec{ humans[i]->getPosition() - m_position };
		float distance{ glm::length(dist_vec) };

		if (distance < smallest_distance)
		{
			smallest_distance = distance;
			closest_human = humans[i];
		}
	}

	return closest_human;
}
