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

private:
	std::vector<std::string> m_level_data{};
	int m_num_humans{};

	TRXEngine::SpriteBatch m_sprite_batch{};

	glm::ivec2 m_player_start_pos{};
	std::vector<glm::ivec2> m_zombie_start_positions{};
};

