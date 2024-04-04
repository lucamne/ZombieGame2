#include "MainGame.h"
#include "Zombie.h"
#include "Gun.h"

#include <TRXEngine/TRXEngine.h>
#include <TRXEngine/Timing.h>
#include <TRXEngine/Errors.h>

#include <iostream>
#include <random>


constexpr float HUMAN_SPEED{ 0.5f };
constexpr float ZOMBIE_SPEED{ 0.8f };

MainGame::MainGame()
	:m_screen_width{ 1024 },
	m_screen_height{768},
	m_fps{ 0 },
	m_player{nullptr},
	m_humans_killed{0},
	m_zombies_killed{0}
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

	m_sprite_batch.init();
}

void MainGame::initLevel()
{
	// level 1
	m_levels.push_back(new Level("Levels/level2.txt"));
	m_current_level = m_levels.back();

	m_player = new Player();
	m_player->init(4.0f, m_current_level->getPlayerStartPosition(),&m_input_manager,&m_camera,&m_bullets);

	m_humans.push_back(m_player);

	std::mt19937 random_engine{ static_cast<unsigned int>(time(nullptr)) };
	std::uniform_int_distribution<int> rand_x(1, m_current_level->getWidth()-2);
	std::uniform_int_distribution<int> rand_y(1, m_current_level->getHeight()-2);



	// add random humans
	for (int i{ 0 }; i < m_current_level->getNumHumans(); i++)
	{
		m_humans.push_back(new Human{});
		glm::vec2 pos(rand_x(random_engine), rand_y(random_engine));
		m_humans.back()->init(HUMAN_SPEED, pos * static_cast<float>(TILE_WIDTH));
	}

	// add zombies
	const std::vector<glm::vec2>& zombie_positions{ m_current_level->getZombieStartPositions() };
	for (int i{ 0 }; i < zombie_positions.size(); i++)
	{
		m_zombies.push_back(new Zombie{});
		m_zombies.back()->init(ZOMBIE_SPEED, zombie_positions[i]);
		
	}

	// add guns
	m_player->addGun(new Gun("Magnum", 10, 1, 0.1f, 30.0f, 20.0f));
	m_player->addGun(new Gun("Shotgun", 30, 20, 0.4f, 30.0f, 20.0f));
	m_player->addGun(new Gun("MP5", 2, 1, 0.2f, 30.0f, 20.0f));

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

	TRXEngine::FpsLimiter fps_limiter{};
	fps_limiter.setMaxFPS(1000.0f);

	while (m_game_state == GAME_STATE::PLAY)
	{
		fps_limiter.begin();

		checkVictory();

		processInput();

		updateAgents();

		updateBullets();
		// update camera position
		m_camera.setPosition(m_player->getPosition());
		m_camera.update();


		drawGame();

		m_fps = fps_limiter.end();

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

	// update zombies
	for (Zombie* zombie : m_zombies)
	{
		zombie->update(m_current_level->getLevelData(), m_humans, m_zombies);
	}

	// update zombie collis9ions
	for (int i{ 0 }; i < m_zombies.size(); i++)
	{
		// collide with other zombies
		for (int j{ i + 1 }; j < m_zombies.size(); j++)
		{
			m_zombies[i]->collideWithAgent(*m_zombies[j]);
		}
		// collide with other humans
		for (int j{ 1 }; j < m_humans.size(); j++)
		{
			if (m_zombies[i]->collideWithAgent(*m_humans[j]))
			{
				// add new zombie
				m_zombies.push_back(new Zombie{});
				m_zombies.back()->init(ZOMBIE_SPEED,m_humans[j]->getPosition());
				// delete the human
				delete m_humans[j];
				m_humans[j] = m_humans.back();
				m_humans.pop_back();
			}
		}

		// collide with player

		if (m_zombies[i]->collideWithAgent(*m_player))
		{
			TRXEngine::fatalError("YOU LOSE");

		}
	}

	// update human collis9ions
	for (int i{ 0 }; i < m_humans.size(); i++)
	{
		// collide with other humans
		for (int j{ i + 1 }; j < m_humans.size(); j++)
		{
			m_humans[i]->collideWithAgent(*m_humans[j]);
		}
	}

}

void MainGame::updateBullets()
{
	// update and collide with world
	for (int i{0}; i < m_bullets.size();)
	{
		// if bullet collided with a wall
		if (m_bullets[i].update(m_current_level->getLevelData()))
		{
			m_bullets[i] = m_bullets.back();
			m_bullets.pop_back();
		}
		else
		{
			i++;
		}
	}

	bool was_bullet_removed{ false };

	// collide with humans and zombies
	for (int i{ 0 }; i < m_bullets.size(); i++)
	{
		was_bullet_removed = false;
		// check for collision with zombies
		for (int j{ 0 }; j < m_zombies.size();)
		{
			// if bullet collides with zombie damage it and kill it if it is out of health
			if (m_bullets[i].collideWithAgent(*m_zombies[j]))
			{
				if (m_zombies[j]->applyDamage(m_bullets[i].getDamage()))
				{
					delete m_zombies[j];
					m_zombies[j] = m_zombies.back();
					m_zombies.pop_back();

					m_zombies_killed++;
				}
				else
				{
					j++;
				}

				m_bullets[i] = m_bullets.back();
				m_bullets.pop_back();
				was_bullet_removed = true;
				break;
			}
			else
			{
				j++;
			}
		}
		// if bullet was removed no need to check collision with humans
		if (was_bullet_removed) continue;

		// check for collision with humans
		// start at 1 to exclude player from collision
		for (int j{ 1 }; j < m_humans.size();)
		{
			// if bullet collides with human damage it and kill it if it is out of health
			if (m_bullets[i].collideWithAgent(*m_humans[j]))
			{
				if (m_humans[j]->applyDamage(m_bullets[i].getDamage()))
				{
					delete m_humans[j];
					m_humans[j] = m_humans.back();
					m_humans.pop_back();

					m_humans_killed++;
				}
				else
				{
					j++;
				}

				m_bullets[i] = m_bullets.back();
				m_bullets.pop_back();
				break;
			}
			else
			{
				j++;
			}
		}
	}
}

void MainGame::checkVictory()
{
	// TODO: support for more levels
	// if zombies are dead we win
	if (m_zombies.size() == 0)
	{

		std::printf("*** You Win! ***\nYou killed %d civilians and %d zombies. There are %zd/%d civilians remaining",m_humans_killed,m_zombies_killed, m_humans.size()-1, m_current_level->getNumHumans());

		TRXEngine::fatalError("");

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

	m_sprite_batch.begin();
	for (Human* human : m_humans)
	{
		human->draw(m_sprite_batch);
	}

	for (Zombie* zombie : m_zombies)
	{
		zombie->draw(m_sprite_batch);
	}

	for (Bullet bullet : m_bullets)
	{
		bullet.draw(m_sprite_batch);
	}

	m_current_level->draw(m_sprite_batch);

	m_sprite_batch.end();
	m_sprite_batch.renderBatch();

	m_texture_program.unuse();

	m_window.swapBuffer();
}
