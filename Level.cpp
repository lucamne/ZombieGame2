#include "Level.h"

#include <TRXEngine/Errors.h>
#include <TRXEngine/ResourceManager.h>

#include <fstream>
#include <iostream>

Level::Level(const std::string& file_name)
{
	std::ifstream file{};
	file.open(file_name);

	// error checking
	if (file.fail())
	{
		TRXEngine::fatalError("Failed to opem " + file_name);
	}

	// throw away the first string in temp
	std::string temp{};
	file >> temp >> m_num_humans;

	std::getline(file, temp); // throw away rest of first line

	while (std::getline(file, temp))
	{
		m_level_data.push_back(temp);
	}
	
	for (int y{ 0 }; y < m_level_data.size(); y++)
	{
		for (int x{ 0 }; x < m_level_data[y].size(); x++)
		{
			// get tile
			char tile{ m_level_data[y][x] };

			// get dest rect
			glm::vec4 dest_rect{ x * TILE_WIDTH,y * TILE_WIDTH,TILE_WIDTH,TILE_WIDTH };

			// process the tile
			switch (tile)
			{
			case '.':
				break;
			case 'R':
				break;
			case 'G':
				break;
			case 'L':
				break;
			case '@':
				m_level_data[y][x] = '.'; /// < reset char value so that collisions are not detected with the '@' sign
				m_player_start_pos = { x * TILE_WIDTH,y * TILE_WIDTH };
				break;
			case 'Z':
				m_level_data[y][x] = '.'; /// < reset char value so that collisions are not detected with the 'z' sign
				m_zombie_start_positions.emplace_back(x * TILE_WIDTH, y * TILE_WIDTH);
				break;
			default:
				std::printf("Unexpected symbol %c at (%d,%d)", tile, x, y);
				break;
			}
		}
	}
	
}

Level::~Level()
{
}

void Level::draw(TRXEngine::SpriteBatch& sprite_batch)
{
	glm::vec4 uv_rect{ 0.0f,0.0f,1.0f,1.0f };
	TRXEngine::Color white_color{ 255,255,255,255 };

	for (int y{ 0 }; y < m_level_data.size(); y++)
	{
		for (int x{ 0 }; x < m_level_data[y].size(); x++)
		{
			// get tile
			char tile{ m_level_data[y][x] };

			// get dest rect
			glm::vec4 dest_rect{ x * TILE_WIDTH,y * TILE_WIDTH,TILE_WIDTH,TILE_WIDTH };

			// process the tile
			switch (tile)
			{
			case '.':
				break;
			case 'R':
				sprite_batch.draw
				(
					dest_rect,
					uv_rect,
					TRXEngine::ResourceManager::getTexture("Textures/wall.png").id,
					0.0f,
					white_color
				);
				break;

			case 'G':
				sprite_batch.draw
				(
					dest_rect,
					uv_rect,
					TRXEngine::ResourceManager::getTexture("Textures/wall.png").id,
					0.0f,
					white_color
				);
				break;

			case 'L':
				sprite_batch.draw
				(
					dest_rect,
					uv_rect,
					TRXEngine::ResourceManager::getTexture("Textures/wall.png").id,
					0.0f,
					white_color
				);
				break;

			case '@':
				m_level_data[y][x] = '.'; /// < reset char value so that collisions are not detected with the '@' sign
				m_player_start_pos = { x * TILE_WIDTH,y * TILE_WIDTH };
				break;

			case 'Z':
				m_level_data[y][x] = '.'; /// < reset char value so that collisions are not detected with the 'z' sign
				m_zombie_start_positions.emplace_back(x, y);
				break;
			default:
				std::printf("Unexpected symbol %c at (%d,%d)", tile, x, y);
				break;
			}
		}
	}

}
