#include <vector>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>

#include "Math.hpp"
#include "RenderWindow.hpp"
#include "Game.hpp"
#include "utils.hpp"

#include "Entity.hpp"
	#include "Ball.hpp"
	#include "Paddle.hpp"
	#include "Barrier.hpp"
#include "Tile.hpp"


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

void Game::add_score(Vector2i& score_add)
{
	score.x += score_add.x;
	score.y += score_add.y;
}