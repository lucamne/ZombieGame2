#define GLM_ENABLE_EXPERIMENTAL

#include "Human.h"

#include <glm/gtx/rotate_vector.hpp>

#include <ctime>
#include <random>

Human::Human()
	:m_direction{ 0 },
	m_frames{ 0 }
{
}

Human::~Human()
{
}

void Human::init(float speed, glm::vec2 pos)
{
	// random number engine and standard distribution used for generating random human direction
	static std::mt19937 random_engine{ static_cast<unsigned int>(time(nullptr)) };
	static std::uniform_real_distribution<float> dist(-1.0f,1.0f);

	m_color = { 255,0,0,255 };
	m_speed = speed;
	m_position = pos;
	// get random direction
	m_direction = glm::vec2(dist(random_engine), dist(random_engine));
	// make sure direction is not 0
	if (m_direction.length() == 0)
	{
		m_direction = glm::vec2(1.0f, 0.0f);
	}
	// normalize direction
	m_direction = glm::normalize(m_direction);
}

void Human::update( const std::vector<std::string>& level_data, [[maybe_unused]] std::vector<Human*> humans, [[maybe_unused]] std::vector<Zombie*> zombies)
{
	static std::mt19937 random_engine{ static_cast<unsigned int>(time(nullptr)) };
	static std::uniform_real_distribution<float> rand_rotate(-1.0f, 1.0f);

	m_position += m_direction * m_speed;
	
	// randomly change direction every 20 frames
	if (m_frames == 20)
	{
		m_direction = glm::rotate(m_direction, rand_rotate(random_engine));
		m_frames = 0;
	}
	else
	{
		m_frames++;
	}
	
	if (collideWithLevel(level_data))
	{
		m_direction = glm::rotate(m_direction, rand_rotate(random_engine));
	}
}
