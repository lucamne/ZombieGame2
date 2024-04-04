#pragma once

#include <TRXEngine/SpriteBatch.h>

#include <glm/glm.hpp>

#include <string>
#include <vector>

class Zombie;
class Human;
class Agent;

constexpr int BULLET_RADIUS{ 5 };

class Bullet
{
public:
	Bullet(glm::vec2 position, glm::vec2 direction, float damage, float speed);
	~Bullet();

	// handles collision with zombies and humans
	// when update returns true delete bullet
	bool update(const std::vector<std::string>& level_data);

	void draw(TRXEngine::SpriteBatch& sprite_batch);

	bool collideWithAgent(Agent& agent);

	// getters
	float getDamage() const { return m_damage; }

private:
	float m_damage{};
	glm::vec2 m_position{};
	glm::vec2 m_direction{};
	float m_speed{};

	bool collideWithWorld(const std::vector<std::string>& level_data);
};

