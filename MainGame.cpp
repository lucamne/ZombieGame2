#include "MainGame.h"

#include <TRXEngine/TRXEngine.h>
#include <TRXEngine/Timing.h>

#include <iostream>
#include <random>

MainGame::MainGame()
	:m_screen_width{ 1024 },
	m_screen_height{768},
	m_fps{ 0 },
	m_player{nullptr}
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

	initLevel();

	gameLoop();
}

void MainGame::initSystems()
{
	TRXEngine::init();

	m_window.create("ZombieGame", m_screen_width, m_screen_height, 0);
	// change background color to light gray
	glClearColor(0.7f, 0.7f, 0.7f, 1.0f);

	m_camera.init(m_screen_width, m_screen_height);

	initShaders();

	m_agent_sprite_batch.init();
}

void MainGame::initLevel()
{
	// level 1
	m_levels.push_back(new Level("Levels/level2.txt"));
	m_current_level = m_levels.back();

	m_player = new Player();
	m_player->init(4.0f, m_current_level->getPlayerStartPosition(),&m_input_manager);

	m_humans.push_back(m_player);

	std::mt19937 random_engine{ static_cast<unsigned int>(time(nullptr)) };
	std::uniform_int_distribution<int> rand_x(1, m_current_level->getWidth()-2);
	std::uniform_int_distribution<int> rand_y(1, m_current_level->getHeight()-2);

	constexpr float HUMAN_SPEED{ 1.0f };

	// add random humans
	for (int i{ 0 }; i < m_current_level->getNumHumans(); i++)
	{
		m_humans.push_back(new Human{});
		glm::vec2 pos(rand_x(random_engine), rand_y(random_engine));
		m_humans.back()->init(HUMAN_SPEED, pos * static_cast<float>(TILE_WIDTH));
	}
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
	// do not want to output fps every frame so use these two values to determine when to print out fps
	//int total_frames{ 0 };
	//int FPS_PRINT_FREQ{ 60 };

	//TRXEngine::FpsLimiter fps_limiter{};
	//fps_limiter.setMaxFPS(1000.0f);

	while (m_game_state == GAME_STATE::PLAY)
	{
		//fps_limiter.begin();

		processInput();

		updateAgents();

		// update camera position
		m_camera.setPosition(m_player->getPosition());
		m_camera.update();


		drawGame();

		//m_fps = fps_limiter.end();

		// print fps ever FPS_PRINT_FREQ frames
		//if (total_frames % FPS_PRINT_FREQ == 0)
		//{
		//	std::cout << "FPS: " << m_fps << '\n';
		//	// prevent potential overflow
		//	if (total_frames >= FPS_PRINT_FREQ)
		//	{
		//		total_frames = 0;
		//	}
		//}
		//total_frames++;
	}
}

void MainGame::updateAgents()
{
	// update all humans
	for (Human* human : m_humans)
	{
		human->update(m_current_level->getLevelData(),m_humans,m_zombies);
	}

	// dont forget to update zombies
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

	m_agent_sprite_batch.begin();
	for (Human* human : m_humans)
	{
		human->draw(m_agent_sprite_batch);
	}
	m_agent_sprite_batch.end();
	m_agent_sprite_batch.renderBatch();

	m_texture_program.unuse();

	m_window.swapBuffer();
}
