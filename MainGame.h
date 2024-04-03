#pragma once

#include "Level.h"
#include "Player.h"

#include <TRXEngine/Window.h>
#include <TRXEngine/GLSLProgram.h>
#include <TRXEngine/InputManager.h>
#include <TRXEngine/Camera2D.h>
#include <TRXEngine/SpriteBatch.h>


enum class GAME_STATE
{
	PLAY,
	EXIT,
};

class MainGame
{
public:
	MainGame();
	~MainGame();

	// run the game
	void run();
private:
	
	// initializes main systems
	void initSystems();

	// initializes level
	void initLevel();

	// initializes GLSL shaders
	void initShaders();

	// main game loop
	void gameLoop();

	// updates all agents
	void updateAgents();

	// processes inputs
	void processInput();

	// draws game
	void drawGame();

	TRXEngine::Window m_window{};

	TRXEngine::GLSLProgram m_texture_program{};

	TRXEngine::InputManager m_input_manager{};

	TRXEngine::Camera2D m_camera{};

	TRXEngine::SpriteBatch m_sprite_batch{}; ///< sprite batch used for drawing all agents

	std::vector<Level*> m_levels{};
	Level* m_current_level{}; ///< pointer to the currently active level

	int m_screen_width{};
	int m_screen_height{};

	Player* m_player{};
	std::vector<Human*> m_humans{}; /// < vector of all humans
	std::vector<Zombie*> m_zombies{}; ///< vector of all zombies

	GAME_STATE m_game_state{ GAME_STATE::PLAY };

	float m_fps{};
	
};

