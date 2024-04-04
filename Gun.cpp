#define GLM_ENABLE_EXPERIMENTAL

#include "Gun.h"

#include <glm/gtx/rotate_vector.hpp>

#include <random>
#include <ctime>

Gun::Gun(std::string name, int firerate, int bullets_per_shot, float spread, float speed, float damage):
	m_name{name},
	m_firerate{firerate},
	m_bullets_per_shot{bullets_per_shot},
	m_spread{spread},
	m_bullet_damage{damage},
	m_bullet_speed{speed},
	m_frame_counter{0}
{
}

Gun::~Gun()
{
}

void Gun::update(bool is_mouse_pressed, glm::vec2 position, glm::vec2 direction, std::vector<Bullet>& bullets)
{
	if (m_frame_counter++ >= m_firerate && is_mouse_pressed)
	{
		fire(position, direction, bullets);
		m_frame_counter = 0;
	}
}

void Gun::fire(glm::vec2 position, glm::vec2 direction, std::vector<Bullet>& bullets) const
{
	static std::mt19937 random_engine{ static_cast<unsigned int>(time(nullptr)) };
	std::uniform_real_distribution<float> dist(-m_spread, m_spread);

	for (int i{ 0 }; i < m_bullets_per_shot; i++)
	{
		bullets.emplace_back(position,glm::rotate(direction,dist(random_engine)), m_bullet_damage, m_bullet_speed);
	}
}
