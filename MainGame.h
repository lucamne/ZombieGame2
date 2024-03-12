#pragma once

#include "Level.h"

#include <TRXEngine/Window.h>
#include <TRXEngine/GLSLProgram.h>
#include <TRXEngine/InputManager.h>
#include <TRXEngine/Camera2D.h>


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

	// initializes GLSL shaders
	void initShaders();

	// main game loop
	void gameLoop();

	// processes inputs
	void processInput();

	// draws game
	void drawGame();

	TRXEngine::Window m_window{};

	TRXEngine::GLSLProgram m_texture_program{};

	TRXEngine::InputManager m_input_manager{};

	TRXEngine::Camera2D m_camera{};

	std::vector<Level*> m_levels{};
	Level* m_current_level{}; ///< pointer to the currently active level

	int m_screen_width{};
	int m_screen_height{};

	GAME_STATE m_game_state{ GAME_STATE::PLAY };

	float m_fps{};
	
};

