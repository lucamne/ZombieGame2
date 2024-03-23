#include "Player.h"

#include <SDL/SDL.h>

Player::Player()
	:m_input_manager{nullptr}
{
}

Player::~Player()
{
}

void Player::init(float speed, glm::vec2 position, TRXEngine::InputManager* input_manager)
{
	m_speed = speed;
	m_position = position;

	// set color
	m_color.r = 0;
	m_color.g = 0;
	m_color.b = 185;
	m_color.a = 255;

	m_input_manager = input_manager;
}

void Player::update(const std::vector<std::string>& level_data,
	std::vector<Human*> humans,
	std::vector<Zombie*> zombies)
{
	if (m_input_manager->isKeyPressed(SDLK_w))
	{
		m_position.y += m_speed;
	}
	if (m_input_manager->isKeyPressed(SDLK_s))
	{
		m_position.y -= m_speed;
	}
	if (m_input_manager->isKeyPressed(SDLK_a))
	{
		m_position.x -= m_speed;
	}
	if (m_input_manager->isKeyPressed(SDLK_d))
	{
		m_position.x += m_speed;
	}

	collideWithLevel(level_data);
}
