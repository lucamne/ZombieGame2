#pragma once

#include "Human.h"
#include "Gun.h"
#

#include <TRXEngine/InputManager.h>
#include <TRXEngine/Camera2D.h>


class Player : public Human
{
public:
	Player();
	~Player();

	void init(float speed, glm::vec2 position, TRXEngine::InputManager* input_manager, TRXEngine::Camera2D* camera, std::vector<Bullet>* bullets);

	void addGun(Gun* gun);

	// moves player based on keyboard input
	// checks player collisions
	void update(const std::vector<std::string>& level_data,
		std::vector<Human*>& humans,
		std::vector<Zombie*>& zombies);

private:
	TRXEngine::InputManager* m_input_manager{};
	TRXEngine::Camera2D* m_camera{};

	std::vector<Gun*> m_guns{};
	std::vector<Bullet>* m_bullets{};
	int m_current_gun_index{};
};

