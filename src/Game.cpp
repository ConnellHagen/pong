#include <vector>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <string>

#include "Game.hpp"
#include "Math.hpp"
#include "RenderWindow.hpp"
#include "utils.hpp"

#include "Entity.hpp"
	#include "Ball.hpp"
	#include "Paddle.hpp"
	#include "Barrier.hpp"
#include "Tile.hpp"
#include "GUI.hpp"

//Game is the container of `Entity`s and the manager of properties relating to an instance of a game (like score)
Game::Game(RenderWindow& window, Entity* p_canvas, const int& p_map, const int& p_score_to_win)
:canvas(p_canvas), current_map(p_map), score(Vector2i(0, 0)), score_to_win(p_score_to_win) //, winner(0), score_board(GUI(window))
{
	ball_t = window.load_texture("res/images/ball.png");
	paddle_t = window.load_texture("res/images/paddle.png");
	barrier_t = window.load_texture("res/images/barrier.png");
	barrier_large_t = window.load_texture("res/images/barrier_large.png");

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
}

Game::~Game()
{}

void Game::restart_game()
{
	init_entity_list();
	init_ball_list();
	init_paddle_list();
	init_barrier_list();
}

void Game::init_entity_list()
{
	entity_list.clear();
}

void Game::init_ball_list()
{
	ball_list.clear();

	ball_list =
	{
		Ball(
			Vector2f(utils::display_width()/2, utils::display_height()/2),
			Vector2f(2, 2),
			ball_t, ball_inf, ball_inf, 5
		)
	};
}

void Game::init_paddle_list()
{
	paddle_list.clear();

	paddle_list =
	{
		Paddle(Vector2f(50, utils::display_height()/2),
			Vector2f(1, 1),
			paddle_t, paddle_inf, paddle_inf, 4
		),
		Paddle(Vector2f(utils::display_width() - 50, utils::display_height()/2),
			Vector2f(1, 1),
			paddle_t, paddle_inf, paddle_inf, 6
		)
	};
	
}

void Game::init_barrier_list()
{
	barrier_list.clear();

	switch(current_map)
	{
	case 0:
		break;
	case 1:
		barrier_list =
		{
			Barrier(
				Vector2f(utils::display_width()/2, 0),
				Vector2f(1, 1),
				barrier_large_t, barrier_large_infsheet, barrier_large_inf, 2
			),
			Barrier(
				Vector2f(utils::display_width()/2, utils::display_height()),
				Vector2f(1, 1),
				barrier_large_t, barrier_large_infsheet, barrier_large_inf, 8
			),
			Barrier(
				Vector2f(utils::display_width()/3, utils::display_height()/3),
				Vector2f(1, 1),
				barrier_t, barrier_infsheet, barrier_inf, 5
			),
			Barrier(
				Vector2f(utils::display_width()*2/3, utils::display_height()*2/3),
				Vector2f(1, 1),
				barrier_t, barrier_infsheet, barrier_inf, 5
			)
		};
		break;
	}
}

void Game::add_score(Vector2i score_add)
{
	score.x += score_add.x;
	score.y += score_add.y;
}

void Game::respawn_ball(int ball_index)
{
	ball_list.erase(ball_list.begin() + ball_index);
	ball_respawn_timers.push_back(Timer(3.0f));
}

void Game::update_timers(const float& delta_time)
{
	for(int i = 0; (unsigned)i < ball_respawn_timers.size(); i++)
	{
		ball_respawn_timers[i].decrease(delta_time);
		if(ball_respawn_timers[i].is_timer_done())
		{
			ball_list.push_back(Ball(Vector2f(utils::display_width()/2, utils::display_height()/2), Vector2f(2, 2), ball_t, ball_inf, ball_inf, 5));
			ball_respawn_timers.erase(ball_respawn_timers.begin() + i);
			i--;
		}
	}
}

std::vector<BUTTON_FUNCTION> Game::update(const std::vector<bool>& key_pushes, const Vector2i& mouse_coords, const float& delta_time)
{
	std::vector<BUTTON_FUNCTION> functions;

	update_timers(delta_time);

	int ball_index = 0;
	for(Ball& temp_ball : ball_list)
	{
		const GOAL_SCORE goal = temp_ball.update(canvas, paddle_list, barrier_list, delta_time);
		switch(goal)
		{
		case NO_SCORE:
			break;

		case P1_SCORE: 
			functions.push_back(PLAYER_1_GOAL);

			add_score(Vector2i(1, 0));
			respawn_ball(ball_index);

			if(score.x >= score_to_win)
			{
				functions.push_back(PLAYER_1_WIN);
			}

			break;
	
		case P2_SCORE: 
			functions.push_back(PLAYER_2_GOAL);

			add_score(Vector2i(0, 1));
			respawn_ball(ball_index);

			if(score.y >= score_to_win)
			{
				functions.push_back(PLAYER_2_WIN);
			}

			break;
		
		}
		ball_index++;
	}

	// std::cout << key_pushes[0] << "\n";

	int key_count = 0;
	for(Paddle& temp_paddle : paddle_list)
	{
		std::vector<bool> temp_keys = {key_pushes[key_count], key_pushes[key_count + 1]};
		temp_paddle.update(canvas, ball_list, temp_keys, delta_time);
		key_count += 2;
	}

	for(Barrier& temp_barrier : barrier_list)
	{
		temp_barrier.update(delta_time);
	}

	return functions;
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