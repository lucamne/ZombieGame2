#include "Player.h"

#include <SDL/SDL.h>

Player::Player()
	:m_input_manager{nullptr},
	m_current_gun_index{-1}
{
}

Player::~Player()
{
}

void Player::init(float speed, glm::vec2 position, TRXEngine::InputManager* input_manager, TRXEngine::Camera2D* camera, std::vector<Bullet>* bullets)
{
	m_camera = camera;
	m_speed = speed;
	m_position = position;
	m_bullets = bullets;

	// set color
	m_color.r = 0;
	m_color.g = 0;
	m_color.b = 185;
	m_color.a = 255;

	m_input_manager = input_manager;
}

void Player::addGun(Gun* gun)
{
	m_guns.push_back(gun);

	// if no gun equipped, equip gun

	if (m_current_gun_index == -1)
	{
		m_current_gun_index = 0;
	}
}

void Player::update(const std::vector<std::string>& level_data,
	[[maybe_unused]]std::vector<Human*>& humans,
	[[maybe_unused]]std::vector<Zombie*>& zombies)
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

	if (m_input_manager->isKeyPressed(SDLK_1) && m_guns.size() >= 0)
	{
		m_current_gun_index = 0;
	}
	if (m_input_manager->isKeyPressed(SDLK_2) && m_guns.size() >= 1)
	{
		m_current_gun_index = 1;
	}
	if (m_input_manager->isKeyPressed(SDLK_3) && m_guns.size() >= 2)
	{
		m_current_gun_index = 2;
	}

	if (m_current_gun_index != -1)
	{
		const glm::vec2 mouse_coords{ m_camera->convertScreenToWorld(m_input_manager->getMouseCoords()) };
		const glm::vec2 center_position{ m_position + glm::vec2(AGENT_RADIUS) };
		const glm::vec2 direction{ glm::normalize(mouse_coords - center_position) };


		m_guns[m_current_gun_index]->update(m_input_manager->isKeyPressed(SDL_BUTTON_LEFT),center_position,direction,*m_bullets);
	}

	collideWithLevel(level_data);
}
