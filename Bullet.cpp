#include "Bullet.h"
#include "Agent.h"
#include "Level.h"

#include <TRXEngine/ResourceManager.h>

Bullet::Bullet(glm::vec2 position, glm::vec2 direction, float damage, float speed):
	m_position{position},
	m_direction{direction},
	m_damage{damage},
	m_speed{speed}
{
}

Bullet::~Bullet()
{
}

bool Bullet::update(const std::vector<std::string>& level_data)
{
	m_position += m_direction * m_speed;
	return collideWithWorld(level_data);
}

void Bullet::draw(TRXEngine::SpriteBatch& sprite_batch)
{
	const glm::vec4 dest_rect{ m_position.x + BULLET_RADIUS,m_position.y + BULLET_RADIUS,BULLET_RADIUS * 2,BULLET_RADIUS * 2 };
	constexpr glm::vec4 uv_rect{ 0.0f,0.0f,1.0f,1.0f };
	constexpr TRXEngine::Color color{ 75,75,75,255 };

	sprite_batch.draw(dest_rect, uv_rect, TRXEngine::ResourceManager::getTexture("Textures/jimmy_jump/PNG/Bullet.png").id,0.0f,color);
}

bool Bullet::collideWithAgent([[maybe_unused]]Agent& agent)
{
	const float MIN_DISTANCE{ AGENT_RADIUS + BULLET_RADIUS};

	glm::vec2 center_pos_a{ m_position};
	glm::vec2 center_pos_b{ agent.getPosition() + glm::vec2(AGENT_RADIUS) };

	glm::vec2 dist_vec{ center_pos_a - center_pos_b };

	float distance{ glm::length(dist_vec) };

	float collision_depth{ MIN_DISTANCE - distance };

	// if distance is smaller than MIN_DISTANCE then we have a collision
	if (collision_depth > 0)
	{
		return true;
	}
	return false;
}

bool Bullet::collideWithWorld(const std::vector<std::string>& level_data)
{
	glm::ivec2 grid_position{ std::floor(m_position.x / static_cast<float>(TILE_WIDTH)), std::floor(m_position.y / static_cast<float>(TILE_WIDTH)) };

	// if agent is outside the world return
	if (grid_position.x < 0 || grid_position.x >= level_data[0].size() || grid_position.y < 0 || grid_position.y >= level_data.size())
	{
		return true;
	}

	return  level_data[grid_position.y][grid_position.x] != '.';
}
