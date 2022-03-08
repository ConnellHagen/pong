#pragma once

#include <vector>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>

#include "Math.hpp"
#include "RenderWindow.hpp"
#include "utils.hpp"

#include "Entity.hpp"
	#include "Ball.hpp"
	#include "Paddle.hpp"
	#include "Barrier.hpp"
#include "Tile.hpp"
#include "GUI.hpp"
	

class Game
{
public:
	Game(RenderWindow& window, Entity* p_canvas, const int& p_map, const int& p_score_to_win);
	~Game();

	void init_entity_list();
	void init_ball_list();
	void init_paddle_list();
	void init_barrier_list();
	void init_background();

	void restart_game();

	inline std::vector<Entity>& get_entity_list(){return entity_list;}
	inline std::vector<Ball>& get_ball_list(){return ball_list;}
	inline std::vector<Paddle>& get_paddle_list(){return paddle_list;}
	inline std::vector<Barrier>& get_barrier_list(){return barrier_list;}
	inline Background& get_background(){return background;}

	inline Vector2i get_score(){return score;}
	void add_score(Vector2i score_add);

	void respawn_ball(int ball_index);
	void update_timers(const float& delta_time);
	void clear_ball_timers();

	std::vector<BUTTON_FUNCTION> update(const std::vector<bool>& key_pushes, const Vector2i& mouse_coords, const float& delta_time);
	void render(RenderWindow& window);


	Entity* canvas;

	SDL_Texture* background_t;
	SDL_Texture* ball_t;
	SDL_Texture* paddle_t;
	SDL_Texture* barrier_t;
	SDL_Texture* barrier_large_t;

	SDL_Rect background_inf;
	SDL_Rect ball_inf;
	SDL_Rect paddle_inf;
	SDL_Rect barrier_inf;
	SDL_Rect barrier_infsheet;
	SDL_Rect barrier_large_inf;
	SDL_Rect barrier_large_infsheet;

private:
	int current_map;
	Vector2i score;
	int score_to_win;
	int ball_respawns_left = 0;

	Background background;
	std::vector<Entity> entity_list;
	std::vector<Ball> ball_list;
	std::vector<Paddle> paddle_list;
	std::vector<Barrier> barrier_list;
	std::vector<Timer> ball_respawn_timers;

};