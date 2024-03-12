#include "MainGame.h"

#include <TRXEngine/TRXEngine.h>
#include <TRXEngine/Timing.h>

#include <iostream>

MainGame::MainGame()
	:m_screen_width{ 1024 },
	m_screen_height{768},
	m_fps{ 0 }
{
}

MainGame::~MainGame()
{
	for (int i{ 0 }; i < m_levels.size(); i++)
	{
		delete m_levels[i];
	}
}

void MainGame::run()
{
	initSystems();

	gameLoop();
}

void MainGame::initSystems()
{
	TRXEngine::init();

	m_window.create("ZombieGame", m_screen_width, m_screen_height, 0);

	m_camera.init(m_screen_width, m_screen_height);

	initShaders();

	m_levels.push_back(new Level("Levels/level1.txt"));
	m_current_level = m_levels.back();
}

void MainGame::initShaders()
{
	// compile color shaders
	m_texture_program.compileShaders("Shaders/colorShading.vert","Shaders/colorShading.frag");
	m_texture_program.addAtrribute("vertexPosition");
	m_texture_program.addAtrribute("vertexColor");
	m_texture_program.addAtrribute("vertexUV");
	m_texture_program.linkShaders();
}

void MainGame::gameLoop()
{
	TRXEngine::FpsLimiter fps_limiter{};
	fps_limiter.setMaxFPS(60.0f);

	while (m_game_state == GAME_STATE::PLAY)
	{
		fps_limiter.begin();

		processInput();
		m_camera.update();

		drawGame();

		m_fps = fps_limiter.end();
	}
}

void MainGame::processInput()
{
	SDL_Event evnt{};
	
	while (SDL_PollEvent(&evnt))
	{
		switch (evnt.type)
		{
		case SDL_QUIT:
			// exit the game
			break;
		case SDL_MOUSEMOTION:
			m_input_manager.setMouseCoords(static_cast<float>(evnt.motion.x), static_cast<float>(evnt.motion.y));
			break;
		case SDL_KEYDOWN:
			m_input_manager.pressKey(evnt.key.keysym.sym);
			break;
		case SDL_KEYUP:
			m_input_manager.releaseKey(evnt.key.keysym.sym);
			break;
		case SDL_MOUSEBUTTONDOWN:
			m_input_manager.pressKey(evnt.button.button);
			break;
		case SDL_MOUSEBUTTONUP:
			m_input_manager.releaseKey(evnt.button.button);
			break;
		default:
			break;
		}
	}
}

void MainGame::drawGame()
{
	// set the base depth to 1.0
	glClearDepth(1.0f);
	// Clear the color and depth buffer
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	m_texture_program.use();

	// draw code
	glActiveTexture(GL_TEXTURE0);
	// make sure the shader uses texture 0
	GLint texture_uniform{ m_texture_program.getUniformLocation("playerTexture") };
	glUniform1i(texture_uniform, 0);

	glm::mat4 projection_matrix{ m_camera.getCameraMatrix() };
	GLint p_uniform{ m_texture_program.getUniformLocation("P") };
	glUniformMatrix4fv(p_uniform, 1, GL_FALSE, &projection_matrix[0][0]);

	m_current_level->draw();

	m_texture_program.unuse();

	m_window.swapBuffer();
}
