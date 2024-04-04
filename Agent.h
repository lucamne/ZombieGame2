#pragma once

#include <TRXEngine/SpriteBatch.h>

#include <glm/glm.hpp>

#include <vector>
#include <string>

const float AGENT_WIDTH{ 60.0f };
const float AGENT_RADIUS{ static_cast<float>(AGENT_WIDTH) / 2.0f };


// forward declaration so that header files are not included which could result in circular includes
class Zombie;
class Human;

class Agent
{
public:
	Agent();
	virtual ~Agent();

	virtual void update(const std::vector<std::string>& level_data,
						std::vector<Human*>& humans,
						std::vector<Zombie*>& zombies) = 0;

	bool collideWithLevel(const std::vector<std::string>& level_data);

	bool collideWithAgent(Agent& agent);
	// returns true if agent died
	bool applyDamage(float damage);

	// draws agent
	void draw(TRXEngine::SpriteBatch& sprite_batch) const;

	// getters
	const glm::vec2& getPosition() const { return m_position; }
protected:
	
	void checkTilePosition(const std::vector<std::string>& level_data, std::vector<glm::vec2>& collide_tile_positions, float x, float y);

	void collideWithTile(glm::vec2 tile_position);

	glm::vec2 m_position{};
	TRXEngine::Color m_color{};
	float m_speed{};
	float m_health{};
};

