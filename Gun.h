#pragma once

#include "Bullet.h"

#include <glm/glm.hpp>

#include <string>
#include <vector>

class Gun
{
public:
	Gun(std::string name, int firerate, int bullets_per_shot, float spread, float speed, float damage);
	~Gun();

	void update(bool is_mouse_pressed, glm::vec2 position, glm::vec2 direction, std::vector<Bullet>& bullets);
	

private:
	std::string m_name{};

	int m_firerate{}; ///< firerate in terms of frames

	int m_bullets_per_shot{}; ///< how many bullets are fired at a time

	float m_spread{}; ///< bullets per shot

	float m_bullet_speed{}; ///< speed of bullets created by gun

	float m_bullet_damage{}; ///< damage of bullets created by gun

	int m_frame_counter{};

	void fire(glm::vec2 position, glm::vec2 direction, std::vector<Bullet>& bullets) const;
};

