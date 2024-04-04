#include "Agent.h"
#include "Level.h"

#include <TRXEngine/ResourceManager.h>

#include <algorithm>

Agent::Agent()
{
}

Agent::~Agent()
{
}

bool Agent::collideWithLevel(const std::vector<std::string>& level_data)
{
	std::vector<glm::vec2> collide_tile_positions{};

	// check the four corners
	// first corner
	checkTilePosition(level_data, collide_tile_positions, m_position.x, m_position.y);
	// second corner
	checkTilePosition(level_data, collide_tile_positions, m_position.x + AGENT_WIDTH, m_position.y);
	// third corner
	checkTilePosition(level_data, collide_tile_positions, m_position.x, m_position.y + AGENT_WIDTH);
	// fourth corner
	checkTilePosition(level_data, collide_tile_positions, m_position.x + AGENT_WIDTH, m_position.y + AGENT_WIDTH);

	for (int i{ 0 }; i < collide_tile_positions.size(); i++)
	{
		collideWithTile(collide_tile_positions[i]);
	}

	return !(collide_tile_positions.size() == 0);
}

bool Agent::collideWithAgent(Agent& agent)
{
	const float MIN_DISTANCE{ 2 * AGENT_RADIUS };

	glm::vec2 center_pos_a{ m_position + glm::vec2(AGENT_RADIUS) };
	glm::vec2 center_pos_b{ agent.getPosition() + glm::vec2(AGENT_RADIUS) };

	glm::vec2 dist_vec{ center_pos_a - center_pos_b };

	float distance{ glm::length(dist_vec) };

	float collision_depth{ MIN_DISTANCE - distance };

	// if distance is smaller than MIN_DISTANCE then we have a collision
	if (collision_depth > 0)
	{

		glm::vec2 collision_depth_vec{ glm::normalize(dist_vec) * collision_depth };

		m_position += collision_depth_vec / 2.0f;
		agent.m_position -= collision_depth_vec / 2.0f;
		return true;
	}
	return false;

}

bool Agent::applyDamage(float damage)
{
	m_health -= damage;
	return m_health <= 0.0f;
}

void Agent::draw(TRXEngine::SpriteBatch& sprite_batch) const
{
	static GLuint texture_id{ TRXEngine::ResourceManager::getTexture("Textures/jimmy_jump/PNG/CharacterLeft_Standing.png").id };

	static constexpr glm::vec4 uv_rect{ 0.0f,0.0f,1.0f,1.0f };

	glm::vec4 dest_rect{ m_position.x,m_position.y,AGENT_WIDTH,AGENT_WIDTH };

	sprite_batch.draw(dest_rect, uv_rect, texture_id, 0.0f, m_color);
}

void Agent::checkTilePosition(const std::vector<std::string>& level_data, std::vector<glm::vec2>& collide_tile_positions, float x, float y)
{

	glm::vec2 corner_pos{ std::floor(x / static_cast<float>(TILE_WIDTH)),std::floor(y / static_cast<float>(TILE_WIDTH)) };

	// if agent is outside the world return
	if (corner_pos.x < 0 || corner_pos.x >= level_data[0].size() || corner_pos.y < 0 || corner_pos.y >= level_data.size())
	{
		return;
	}

	// if tile is not empty space
	if (level_data[static_cast<int>(corner_pos.y)][static_cast<int>(corner_pos.x)] != '.')
	{
		// push back center position of collided tile in world coordinates
		collide_tile_positions.push_back(corner_pos * static_cast<float>(TILE_WIDTH) + glm::vec2(static_cast<float>(TILE_WIDTH) / 2.0f));
	}
}

// AABB collision
void Agent::collideWithTile(glm::vec2 tile_position)
{
	const float TILE_RADIUS{ static_cast<float>(TILE_WIDTH) / 2.0f };
	const float MIN_DISTANCE{ AGENT_RADIUS + TILE_RADIUS };

	glm::vec2 center_player_pos{ m_position + glm::vec2(AGENT_RADIUS) };
	glm::vec2 dist_vec{ center_player_pos - tile_position };

	float x_depth{ MIN_DISTANCE - std::abs(dist_vec.x) };
	float y_depth{ MIN_DISTANCE - std::abs(dist_vec.y) };

	// if true we are colliding
	if (x_depth > 0 || y_depth > 0)
	{
		if (std::max(x_depth,0.0f) < std::max( y_depth, 0.0f))
		{
			if (dist_vec.x < 0)
			{
				m_position.x -= x_depth;
			}
			else
			{
				m_position.x += x_depth;
			}
		}
		else
		{
			if (dist_vec.y < 0)
			{
				m_position.y -= y_depth;
			}
			else
			{
				m_position.y += y_depth;
			}
		}

	}
}
