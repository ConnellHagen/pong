#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <cmath>

#include "Entity.hpp"
#include "Math.hpp"

#include "Paddle.hpp"
#include "Ball.hpp"

Paddle::Paddle(const Vector2f& p_pos, const Vector2f& p_scale, SDL_Texture* p_texture, const SDL_Rect& p_sheet, const SDL_Rect& p_current, const int& p_render_mode)
	:Entity(p_pos, p_scale, p_texture, p_sheet, p_current, p_render_mode), direction(0), velocity(6.0f)
{}

Vector2f Paddle::next_pos()
{
	Vector2f new_pos = get_pos();
	switch(direction)
	{
		case 0:
			break;
		case 1:
			new_pos.y -= floor(velocity);
			break;
		case 2:
			new_pos.y += floor(velocity);
			break;
	}
	return new_pos;
}

void Paddle::update(Entity& canvas, std::vector<Ball> ball_list, std::vector<bool>& key_pushes)
{
	bool is_UP = key_pushes[0];
	bool is_DOWN = key_pushes[1];

	if(is_UP)
	{
		set_direction(1);
	}
	else if(is_DOWN)
	{
		set_direction(2);
	}
	else
	{
		set_direction(0);
	}


	const SDL_Rect paddle_hitbox = get_border_box();

	Vector2f corner_top(paddle_hitbox.x, paddle_hitbox.y - floor(get_velocity()));
	Vector2f corner_bottom(paddle_hitbox.x, paddle_hitbox.y + paddle_hitbox.h + floor(get_velocity()));

	if(get_direction() == 1 && !canvas.is_point_within(corner_top))
		return;
	if(get_direction() == 2 && !canvas.is_point_within(corner_bottom))
		return;

	SDL_Rect test_up = paddle_hitbox;
	test_up.y -= floor(get_velocity());
	SDL_Rect test_down = paddle_hitbox;
	test_down.y += floor(get_velocity());

	bool dont_update = false;

	for(Ball& temp_ball : ball_list)
	{
		SDL_Rect ball_next = temp_ball.get_border_box();

		if(get_direction() == 1 && game_math::rect_collide(ball_next, test_up))
		{
			// set_direction(2);
			// update();
			// set_direction(1);
			dont_update = true;
		}
		if(get_direction() == 2 && game_math::rect_collide(ball_next, test_down))
		{
			// set_direction(1);
			// update();
			// set_direction(2);
			dont_update = true;
		}
	}

	if(!dont_update)
	{
		Entity::set_pos(next_pos());
	}
	
}