#pragma once

#include <TRXEngine/SpriteBatch.h>

#include <glm/glm.hpp>

#include <string>
#include <vector>

constexpr int TILE_WIDTH{ 64 };

class Level
{
public:
	// load the level
	Level(const std::string& file_name);
	~Level();

	// draw game
	void draw();

	// getters
	glm::vec2 getPlayerStartPosition() const { return m_player_start_pos; }
	const std::vector<glm::vec2>& getZombieStartPositions() const { return m_zombie_start_positions; }
	const std::vector<std::string>& getLevelData() const { return m_level_data; }

private:
	std::vector<std::string> m_level_data{};
	int m_num_humans{};

	TRXEngine::SpriteBatch m_sprite_batch{};

	glm::vec2 m_player_start_pos{};
	std::vector<glm::vec2> m_zombie_start_positions{};
};

