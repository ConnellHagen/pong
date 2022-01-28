#include <vector>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>

#include "Game.hpp"
#include "Math.hpp"
#include "RenderWindow.hpp"
#include "utils.hpp"

#include "Entity.hpp"
	#include "Ball.hpp"
	#include "Paddle.hpp"
	#include "Barrier.hpp"
#include "Tile.hpp"

//Game is the container of `Entity`s and the manager of properties relating to an instance of a game (like score)
Game::Game(const int& p_map, const int& p_score_to_win, RenderWindow& window)
:current_map(p_map), score(Vector2i(0, 0)), score_to_win(p_score_to_win)
{
	background_t = window.load_texture("res/images/background.png");
	ball_t = window.load_texture("res/images/ball.png");
	paddle_t = window.load_texture("res/images/paddle.png");
	barrier_t = window.load_texture("res/images/barrier.png");
	barrier_large_t = window.load_texture("res/images/barrier_large.png");

	background_inf = {0, 0, 32, 32};
	ball_inf = {0, 0, 16, 16};
	paddle_inf = {0, 0, 32, 128};
	barrier_inf = {0, 0, 64, 64};
	barrier_infsheet = {0, 0, 256, 128};
	barrier_large_inf = {0, 0, 128, 128};
	barrier_large_infsheet = {0, 0, 512, 256};

	init_entity_list();
	init_ball_list();
	init_paddle_list();
	init_barrier_list();
	init_background();
}

Game::~Game()
{}

void Game::init_entity_list()
{
	entity_list.clear();
}

void Game::init_ball_list()
{
	ball_list.clear();
	ball_list.push_back(Ball(Vector2f(utils::display_width()/2, utils::display_height()/2), Vector2f(2, 2), ball_t, ball_inf, ball_inf, 5));
}

void Game::init_paddle_list()
{
	paddle_list.clear();
	paddle_list.push_back(Paddle(Vector2f(50, utils::display_height()/2), Vector2f(1, 1), paddle_t, paddle_inf, paddle_inf, 4));
	paddle_list.push_back(Paddle(Vector2f(utils::display_width() - 50, utils::display_height()/2), Vector2f(1, 1), paddle_t, paddle_inf, paddle_inf, 6));
	
}

void Game::init_barrier_list()
{
	barrier_list.clear();
	switch(current_map)
	{
		case 0:
			break;
		case 1:
			barrier_list.clear();
			barrier_list.push_back(Barrier(Vector2f(utils::display_width()/2, 0), Vector2f(1, 1), barrier_large_t, barrier_large_infsheet, barrier_large_inf, 2));
			barrier_list.push_back(Barrier(Vector2f(utils::display_width()/2, utils::display_height()), Vector2f(1, 1), barrier_large_t, barrier_large_infsheet, barrier_large_inf, 8));
			barrier_list.push_back(Barrier(Vector2f(utils::display_width()/3, utils::display_height()/3), Vector2f(1, 1), barrier_t, barrier_infsheet, barrier_inf, 5));
			barrier_list.push_back(Barrier(Vector2f(utils::display_width()*2/3, utils::display_height()*2/3), Vector2f(1, 1), barrier_t, barrier_infsheet, barrier_inf, 5));
			break;
	}
}

void Game::init_background()
{
	background = Background(Vector2f(4, 4), background_t, background_inf);
}

void Game::add_score(Vector2i score_add)
{
	score.x += score_add.x;
	score.y += score_add.y;
}

void Game::respawn_ball(int ball_index)
{
	ball_list.erase(ball_list.begin() + ball_index);
	ball_respawn_timers.push_back(Timer(200.0f));

	
}

void Game::update_timers()
{
	for(int i = 0; (unsigned)i < ball_respawn_timers.size(); i++)
	{
		ball_respawn_timers[i].decrement();
		if(ball_respawn_timers[i].is_timer_done())
		{
			ball_list.push_back(Ball(Vector2f(utils::display_width()/2, utils::display_height()/2), Vector2f(2, 2), ball_t, ball_inf, ball_inf, 5));
			ball_respawn_timers.erase(ball_respawn_timers.begin() + i);
			i--;
		}
	}
}

void Game::update(Entity& canvas, const std::vector<bool>& key_pushes)
{
	update_timers();

	int ball_index = 0;
	for(Ball& temp_ball : ball_list)
	{
		const int goal = temp_ball.update(canvas, paddle_list, barrier_list);
		switch(goal)
		{
			case 1:
				add_score(Vector2i(1, 0));
				respawn_ball(ball_index);
				break;
			case 2:
				add_score(Vector2i(0, 1));
				respawn_ball(ball_index);
				break;
		}
		ball_index++;
	}


	int key_count = 0;
	for(Paddle& temp_paddle : paddle_list)
	{
		std::vector<bool> temp_keys = {key_pushes[key_count], key_pushes[key_count + 1]};
		temp_paddle.update(canvas, ball_list, temp_keys);
		key_count += 2;
	}

	for(Barrier& temp_barrier : barrier_list)
	{
		temp_barrier.update();
	}
}

void Game::render(RenderWindow& window)
{
	window.render(background);

	for(Entity& temp_entity : entity_list)
	{
		window.render(temp_entity);
	}

	for(Ball& temp_ball : ball_list)
	{
		window.render(static_cast<Entity>(temp_ball));
	}

	for(Paddle& temp_paddle : paddle_list)
	{
		window.render(static_cast<Entity>(temp_paddle));
	}

	for(Barrier& temp_barrier : barrier_list)
	{
		window.render(static_cast<Entity>(temp_barrier));
	}
}