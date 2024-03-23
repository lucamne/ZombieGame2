#pragma once

#include "Human.h"

#include <TRXEngine/InputManager.h>


class Player : public Human
{
public:
	Player();
	~Player();

	void init(float speed, glm::vec2 position, TRXEngine::InputManager* input_manager);

	// moves player based on keyboard input
	// checks player collisions
	void update(const std::vector<std::string>& level_data,
		std::vector<Human*> humans,
		std::vector<Zombie*> zombies);

private:
	TRXEngine::InputManager* m_input_manager{};
};

