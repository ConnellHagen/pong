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
 	Entity canvas(Vector2f(0, 0), Vector2f(1, 1), NULL, canvas_inf, canvas_inf, 1);

	RenderWindow window("Game Title", utils::display_width(), utils::display_height(), Vector2f(1, 1));


	SDL_Texture* background_t = window.load_texture("res/images/background.png");
	SDL_Rect background_inf = {0, 0, 32, 32};

	Background background(Vector2f(4, 4), background_t, background_inf);


	SDL_Texture* ball_t = window.load_texture("res/images/ball.png");
	SDL_Rect ball_inf = {0, 0, 16, 16};
	SDL_Texture* paddle_t = window.load_texture("res/images/paddle.png");
	SDL_Rect paddle_inf = {0, 0, 32, 128};
	SDL_Texture* barrier_t = window.load_texture("res/images/barrier.png");
	SDL_Rect barrier_inf = {0, 0, 64, 64};
	SDL_Rect barrier_infsheet = {0, 0, 256, 128};
	SDL_Texture* barrier_large_t = window.load_texture("res/images/barrier_large.png");
	SDL_Rect barrier_large_inf = {0, 0, 128, 128};
	SDL_Rect barrier_large_infsheet = {0, 0, 512, 256};

	vector<Entity> entities = {};

	vector<Ball> balls =
	{
		Ball(Vector2f(utils::display_width()/2, utils::display_height()/2), Vector2f(2, 2), ball_t, ball_inf, ball_inf, 5)
	};

	vector<Paddle> paddles =
	{
		Paddle(Vector2f(50, utils::display_height()/2), Vector2f(1, 1), paddle_t, paddle_inf, paddle_inf, 4),
		Paddle(Vector2f(utils::display_width() - 50, utils::display_height()/2), Vector2f(1, 1), paddle_t, paddle_inf, paddle_inf, 6)
	};

	vector<Barrier> barriers = {
		Barrier(Vector2f(utils::display_width()/2, 0), Vector2f(1, 1), barrier_large_t, barrier_large_infsheet, barrier_large_inf, 2),
		Barrier(Vector2f(utils::display_width()/2, utils::display_height()), Vector2f(1, 1), barrier_large_t, barrier_large_infsheet, barrier_large_inf, 8),
		Barrier(Vector2f(utils::display_width()/3, utils::display_height()/3), Vector2f(1, 1), barrier_t, barrier_infsheet, barrier_inf, 5),
		Barrier(Vector2f(utils::display_width()*2/3, utils::display_height()*2/3), Vector2f(1, 1), barrier_t, barrier_infsheet, barrier_inf, 5)
	};

	//W, S, UP, DOWN
	vector<bool> key_pushes(4, false);


	bool game_running = true;

	SDL_Event event;

	const float time_step = .01;
	float accumulator = 0.0f;
	float current_time = utils::hire_time_in_seconds();

	while(game_running)
	{
		int start_ticks = SDL_GetTicks();

		float new_time = utils::hire_time_in_seconds();
		float frame_time = new_time - current_time;
		current_time = new_time;
		accumulator += frame_time;

		while(accumulator >= time_step)
		{
			while(SDL_PollEvent(&event))
			{
				switch(event.type)
				{
					case SDL_QUIT:
						game_running = false;
						break;

					case SDL_KEYDOWN:
						switch (event.key.keysym.sym)
						{
						    case SDLK_w:
						    	key_pushes[0] = true;
						    	break;

						    case SDLK_s:
						    	key_pushes[1] = true;
						    	break;

						    case SDLK_UP:
							    key_pushes[2] = true;
							    break;

						    case SDLK_DOWN:
							    key_pushes[3] = true;
							    break;
						}
						break;

					case SDL_KEYUP:
						switch (event.key.keysym.sym)
						{
							case SDLK_w:
						    	key_pushes[0] = false;
						    	break;

						    case SDLK_s:
						    	key_pushes[1] = false;
						    	break;

						    case SDLK_UP:
							    key_pushes[2] = false;
							    break;
							    
						    case SDLK_DOWN:
							    key_pushes[3] = false;
							    break;
						}
						break;
				}
			}

			accumulator -= time_step;

		}
		// const float alpha = accumulator / deltaTime;
		// const float delta_time;
		

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

				float percent_from_center = game_math::clamp((temp_paddle.get_center().y - temp_ball.get_center().y) / (temp_paddle.get_border_box().h / 2.0f) * 100, -100, 100);

				if(!was_x_reversed && game_math::rect_collide(test_leftright, temp_paddle.get_border_box()))
				{
					if(temp_ball.get_velocity().x >= 0)
						temp_ball.set_rotation_direction(1);
					else
						temp_ball.set_rotation_direction(0);
					temp_ball.random_rotation_velocity();

					temp_ball.bounce_y_velocity(percent_from_center);

					temp_ball.scale_velocity(Vector2f(-1, 1));
					was_x_reversed = true;
				}
				if(!was_y_reversed && game_math::rect_collide(test_updown, temp_paddle.get_border_box()))
				{
					temp_ball.reverse_rotation_direction();
					temp_ball.random_rotation_velocity();

					temp_ball.scale_velocity(Vector2f(1, -1));
					was_y_reversed = true;
				}
				if(!was_x_reversed && !was_y_reversed && game_math::rect_collide(test_corner, temp_paddle.get_border_box()))
				{
					temp_ball.reverse_rotation_direction();
					temp_ball.random_rotation_velocity();

					temp_ball.bounce_y_velocity(percent_from_center);

					temp_ball.scale_velocity(Vector2f(-1, -1));
					was_x_reversed = true;
					was_y_reversed = true;
				}
			}

			for(Barrier& temp_barrier : barriers)
			{
				float distance_from_xcenter = abs(temp_ball.get_center().x - temp_barrier.get_center().x);
				float distance_from_ycenter = abs(temp_ball.get_center().y - temp_barrier.get_center().y);
				// float percent_from_center_y = game_math::clamp(distance_from_ycenter / (temp_barrier.get_border_box().h / 2.0f) * 100, -100, 100);
				// float percent_from_center_x = game_math::clamp(distance_from_xcenter / (temp_barrier.get_border_box().w / 2.0f) * 100, -100, 100);

				if(!was_x_reversed && game_math::rect_collide(test_leftright, temp_barrier.get_border_box()))
				{
					if(temp_ball.get_velocity().x >= 0)
						temp_ball.set_rotation_direction(1);
					else
						temp_ball.set_rotation_direction(0);
					temp_ball.random_rotation_velocity();

					temp_ball.scale_velocity(Vector2f(-1, 1));
					was_x_reversed = true;
				}
				if(!was_y_reversed && game_math::rect_collide(test_updown, temp_barrier.get_border_box()))
				{
					temp_ball.reverse_rotation_direction();
					temp_ball.random_rotation_velocity();

					temp_ball.scale_velocity(Vector2f(1, -1));
					was_y_reversed = true;
				}
				if(!was_x_reversed && !was_y_reversed && game_math::rect_collide(test_corner, temp_barrier.get_border_box()))
				{
					temp_ball.reverse_rotation_direction();
					temp_ball.random_rotation_velocity();

					if(distance_from_xcenter == distance_from_ycenter)
					{
						temp_ball.scale_velocity(Vector2f(-1, -1));
						was_x_reversed = true;
						was_y_reversed = true;
					}
					else if(distance_from_xcenter > distance_from_ycenter)
					{
						temp_ball.scale_velocity(Vector2f(-1, 1));
						was_x_reversed = true;
					}
					else
					{
						temp_ball.scale_velocity(Vector2f(1, -1));
						was_y_reversed = true;
					}

				}
			}

			temp_ball.update();

		}

		if(key_pushes[0])
			paddles[0].set_direction(1);
		else if(key_pushes[1])
			paddles[0].set_direction(2);
		else
			paddles[0].set_direction(0);
		
		if(key_pushes[2])
			paddles[1].set_direction(1);
		else if(key_pushes[3])
			paddles[1].set_direction(2);
		else
			paddles[1].set_direction(0);
			
		for(Paddle& temp_paddle : paddles)
		{
			const SDL_Rect paddle_hitbox = temp_paddle.get_border_box();

			Vector2f corner_top(paddle_hitbox.x, paddle_hitbox.y);
			Vector2f corner_bottom(paddle_hitbox.x, paddle_hitbox.y + paddle_hitbox.h);

			if(temp_paddle.get_direction() == 1 && !canvas.is_point_within(corner_top))
				continue;
			if(temp_paddle.get_direction() == 2 && !canvas.is_point_within(corner_bottom))
				continue;

			SDL_Rect test_up = temp_paddle.get_border_box();
			test_up.y -= 6;
			SDL_Rect test_down = temp_paddle.get_border_box();
			test_down.y += 6;

			bool dont_update = false;

			for(Ball& temp_ball : balls)
			{
				SDL_Rect ball_next = temp_ball.get_border_box();
				ball_next.x += temp_ball.get_velocity().x;
				ball_next.y += temp_ball.get_velocity().y;

				if(temp_paddle.get_direction() == 1 && game_math::rect_collide(ball_next, test_up))
				{
					temp_paddle.set_direction(2);
					temp_paddle.update();
					temp_paddle.set_direction(1);
					dont_update = true;
				}
				if(temp_paddle.get_direction() == 2 && game_math::rect_collide(ball_next, test_down))
				{
					temp_paddle.set_direction(1);
					temp_paddle.update();
					temp_paddle.set_direction(2);
					dont_update = true;
				}

			}

			if(!dont_update)
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