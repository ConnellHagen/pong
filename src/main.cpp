#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <iostream>
#include <vector>

#include "RenderWindow.hpp"
#include "Entity.hpp"
#include "Paddle.hpp"
#include "Ball.hpp"
#include "Tile.hpp"
#include "Barrier.hpp"

#include "Math.hpp"
#include "utils.hpp"

using std::vector;

int main(int argc, char* args[])
{
	if(SDL_Init(SDL_INIT_VIDEO) > 0)
	{
		std::cout << "SDL_Init has failed. SDL_ERROR: " << SDL_GetError() << "\n";
	}

	if(!(IMG_Init(IMG_INIT_PNG)))
		std::cout << "IMG_Init has failed. Error: " << SDL_GetError() << "\n";


	SDL_Rect canvas_inf = {0, 0, utils::display_width(), utils::display_height()};
 	Entity canvas(Vector2f(0, 0), Vector2f(1, 1), NULL, canvas_inf);

	RenderWindow window("Game Title", utils::display_width(), utils::display_height(), Vector2f(1, 1));


	SDL_Texture* background_t = window.load_texture("res/images/background.png");
	SDL_Rect background_inf = {0, 0, 32, 32};

	Background background(Vector2f(4, 4), background_t, background_inf);


	SDL_Texture* ball_t = window.load_texture("res/images/ball.png");
	SDL_Rect ball_inf = {0, 0, 16, 16};
	SDL_Texture* paddle_t = window.load_texture("res/images/paddle.png");
	SDL_Rect paddle_inf = {0, 0, 32, 128};
	// SDL_Texture* barrier_t = window.load_texture("res/images/barrier.png");
	// SDL_Rect barrier_inf = {0, 0, 64, 64};
	// SDL_Texture* barrier_large_t = window.load_texture("res/images/barrier.png");
	// SDL_Rect barrier_large_inf = {0, 0, 128, 128};

	vector<Entity> entities = {};

	vector<Ball> balls =
	{
		Ball(Vector2f(utils::display_width()/2, utils::display_height()/2), Vector2f(2, 2), ball_t, ball_inf)
	};

	vector<Paddle> paddles =
	{
		Paddle(Vector2f(50, utils::display_height()/2), Vector2f(2, 2), paddle_t, paddle_inf),
		Paddle(Vector2f(utils::display_width() - (50 + 2 * paddle_inf.w), utils::display_height()/2), Vector2f(2, 2), paddle_t, paddle_inf)
	};

	vector<Barrier> barriers = {};


	bool game_running = true;

	SDL_Event event;

	const float delta_time = .01;
	float accumulator = 0.0f;
	float current_time = utils::hire_time_in_seconds();

	while(game_running)
	{
		int start_ticks = SDL_GetTicks();

		float new_time = utils::hire_time_in_seconds();
		float frame_time = new_time - current_time;
		current_time = new_time;
		accumulator += frame_time;

		while(accumulator >= delta_time)
		{
			while(SDL_PollEvent(&event))
			{
				if(event.type == SDL_QUIT)
					game_running = false;
			}

			accumulator -= delta_time;

		}
		// const float alpha = accumulator / deltaTime;
		

		window.clear();


		//updating
		for(Ball& temp_ball : balls)
		{

			bool was_x_reversed = false;
			bool was_y_reversed = false;

			const SDL_Rect ball_hitbox = temp_ball.get_border_box();

			Vector2f corner_tl(ball_hitbox.x, ball_hitbox.y);
			Vector2f corner_tr(ball_hitbox.x + ball_hitbox.w, ball_hitbox.y);
			Vector2f corner_bl(ball_hitbox.x, ball_hitbox.y + ball_hitbox.h);
			Vector2f corner_br(ball_hitbox.x + ball_hitbox.w, ball_hitbox.y + ball_hitbox.h);

			if(!was_x_reversed && !canvas.is_point_within(corner_tl) && !canvas.is_point_within(corner_bl))
			{
				temp_ball.scale_velocity(Vector2f(-1, 1));
				was_x_reversed = true;
			}
			else if(!was_x_reversed && !canvas.is_point_within(corner_tr) && !canvas.is_point_within(corner_br))
			{
				temp_ball.scale_velocity(Vector2f(-1, 1));
				was_x_reversed = true;
			}
			if(!was_y_reversed && !canvas.is_point_within(corner_tl) && !canvas.is_point_within(corner_tr))
			{
				temp_ball.scale_velocity(Vector2f(1, -1));
				was_y_reversed = true;
			}
			else if(!was_y_reversed && !canvas.is_point_within(corner_bl) && !canvas.is_point_within(corner_br))
			{
				temp_ball.scale_velocity(Vector2f(1, -1));
				was_y_reversed = true;
			}


			SDL_Rect test_leftright = ball_hitbox;
			test_leftright.x += temp_ball.get_velocity().x;
			SDL_Rect test_updown = ball_hitbox;
			test_updown.y += temp_ball.get_velocity().y;
			SDL_Rect test_corner = ball_hitbox;
			test_corner.x += temp_ball.get_velocity().x;
			test_corner.y += temp_ball.get_velocity().y;

			for(Paddle& temp_paddle : paddles)
			{
				if(!was_x_reversed && game_math::rect_collide(test_leftright, temp_paddle.get_border_box()))
				{
					temp_ball.scale_velocity(Vector2f(-1, 1));
					was_x_reversed = true;
				}
				if(!was_y_reversed && game_math::rect_collide(test_updown, temp_paddle.get_border_box()))
				{
					temp_ball.scale_velocity(Vector2f(1, -1));
					was_y_reversed = true;
				}
				if(!was_x_reversed && !was_y_reversed && game_math::rect_collide(test_corner, temp_paddle.get_border_box()))
				{
					temp_ball.scale_velocity(Vector2f(-1, -1));
					was_x_reversed = true;
					was_y_reversed = true;
				}
			}

			for(Barrier& temp_barrier : barriers)
			{
				if(!was_x_reversed && game_math::rect_collide(test_leftright, temp_barrier.get_border_box()))
				{
					temp_ball.scale_velocity(Vector2f(-1, 1));
					was_x_reversed = true;
				}
				if(!was_y_reversed && game_math::rect_collide(test_updown, temp_barrier.get_border_box()))
				{
					temp_ball.scale_velocity(Vector2f(1, -1));
					was_y_reversed = true;
				}
				if(!was_x_reversed && !was_y_reversed && game_math::rect_collide(test_corner, temp_barrier.get_border_box()))
				{
					temp_ball.scale_velocity(Vector2f(-1, -1));
					was_x_reversed = true;
					was_y_reversed = true;
				}
			}


			temp_ball.update();

		}

		for(Paddle& temp_paddle : paddles)
		{
			temp_paddle.update();
		}

		for(Barrier& temp_barrier : barriers)
		{
			temp_barrier.update();
		}

		//rendering
		window.render(background);

		for(Entity& temp_entity : entities)
		{
			window.render(temp_entity);
		}

		for(Ball& temp_ball : balls)
		{
			window.render(static_cast<Entity>(temp_ball));
		}

		for(Paddle& temp_paddle : paddles)
		{
			window.render(static_cast<Entity>(temp_paddle));
		}

		for(Barrier& temp_barrier : barriers)
		{
			window.render(static_cast<Entity>(temp_barrier));
		}


		//displaying
		window.display();

		int frame_ticks = SDL_GetTicks() - start_ticks;

		if(frame_ticks < 1000 / window.get_refresh_rate())
			SDL_Delay(1000 / window.get_refresh_rate() - frame_ticks);

	}

	window.clean_up();
	SDL_Quit();

	return 0;
}