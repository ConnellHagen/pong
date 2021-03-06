#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <cmath>

#include "Entity.hpp"
#include "Math.hpp"

#include "Paddle.hpp"
#include "Ball.hpp"

Paddle::Paddle(const Vector2f& p_pos, const Vector2f& p_scale, SDL_Texture* p_texture, const SDL_Rect& p_sheet, const SDL_Rect& p_current, const int& p_render_mode)
	:Entity(p_pos, p_scale, p_texture, p_sheet, p_current, p_render_mode), direction(NONE), velocity(280.0f)
{}

Vector2f Paddle::next_pos(const float& delta_time)
{
	Vector2f new_pos = get_pos();

	float change = floor(velocity) * delta_time;

	switch(direction)
	{
	case NONE:
		break;
	case UP:
		new_pos.y -= change;
		break;
	case DOWN:
		new_pos.y += change;
		break;
	}

	return new_pos;
}

void Paddle::update(Entity* canvas, std::vector<Ball> ball_list, std::vector<bool>& key_pushes, const float& delta_time)
{
	bool is_UP = key_pushes[0];
	bool is_DOWN = key_pushes[1];

	if(is_UP)
	{
		direction = UP;
	}
	else if(is_DOWN)
	{
		direction = DOWN;
	}
	else
	{
		direction = NONE;
	}

	const SDL_Rect paddle_hitbox = get_border_box();

	Vector2f corner_top(paddle_hitbox.x, paddle_hitbox.y - floor(velocity) * delta_time);
	Vector2f corner_bottom(paddle_hitbox.x, paddle_hitbox.y + paddle_hitbox.h + floor(velocity) * delta_time);

	if(direction == UP && !canvas->is_point_within(corner_top))
		return;
	if(direction == DOWN && !canvas->is_point_within(corner_bottom))
		return;

	bool dont_update = false;

	SDL_Rect test_up = paddle_hitbox;
	test_up.y -= floor(velocity);
	SDL_Rect test_down = paddle_hitbox;
	test_down.y += floor(velocity);

	for(Ball& temp_ball : ball_list)
	{
		SDL_Rect ball_next = temp_ball.get_border_box();

		if((get_direction() == 1 && game_math::rect_collide(ball_next, test_up)) ||
		   (get_direction() == 2 && game_math::rect_collide(ball_next, test_down)))
		{
			dont_update = true;
		}
	}

	if(!dont_update)
	{
		Entity::set_pos(next_pos(delta_time));
	}
	
}