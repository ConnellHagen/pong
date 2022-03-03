#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <vector>
#include <ctime>
#include <cmath>

#include "Math.hpp"
#include "utils.hpp"

#include "Entity.hpp"
	#include "Barrier.hpp"
	#include "Paddle.hpp"
	#include "Ball.hpp"

Ball::Ball(const Vector2f& p_pos, const Vector2f& p_scale, SDL_Texture* p_texture, const SDL_Rect& p_sheet, const SDL_Rect& p_current, const int& p_render_mode)
	:Entity(p_pos, p_scale, p_texture, p_sheet, p_current, p_render_mode), net_velocity(6)
{
	velocity = Vector2f(utils::random(-1, 0), utils::random(-3, 3));
	update_x_velocity();
	random_rotation_velocity();
}

void Ball::bounce_y_velocity(float& p_percent_off)
{
	if(p_percent_off == 0)
	{
		velocity.y = 0;
	}
	else
	{
		float percent_off = p_percent_off;
		float y_vel = ceil(percent_off / 10.0) / 2.0f;
		if(velocity.y < 0)
		{
			y_vel *= -1;
		}
		velocity.y = y_vel;
	}
	update_x_velocity();
}

void Ball::update_x_velocity()
{
	if(velocity.y == 0)
	{
		velocity.x = net_velocity;
		return;
	}
	bool is_negative = velocity.x < 0;
	float x_squared = pow(net_velocity, 2) - pow(velocity.y, 2);
	float x = sqrt(x_squared);
	if(is_negative)
		x *= -1;
	velocity.x = x;
}

void Ball::scale_velocity(const Vector2f& p_scale)
{
	velocity.x *= p_scale.x;
	velocity.y *= p_scale.y;
}

void Ball::set_velocity_dir(const Vector2f& p_direction)
{
	velocity.x = abs(velocity.x);
	velocity.y = abs(velocity.y);

	velocity.x *= p_direction.x;
	velocity.y *= p_direction.y;
}


void Ball::random_rotation_velocity()
{
	bool is_negative = rotation_velocity < 0;
    rotation_velocity = utils::random(1, 7);
    if(is_negative)
    	rotation_velocity *= -1;
}

//0 for right, 1 for left
void Ball::set_rotation_direction(const int& direction)
{
	if((direction == 0 && rotation_velocity >= 0) || (direction == 1 && rotation_velocity < 0))
		return;
	else 
		rotation_velocity *= -1;
}

int Ball::get_rotation_direction()
{
	if(rotation_velocity >= 0)
		return 0;
	else
		return 1;
}

Vector2f Ball::next_pos()
{
	Vector2f new_pos = get_pos();
	new_pos.x += velocity.x;
	new_pos.y += velocity.y;
	return new_pos;
}

GOAL_SCORE Ball::update(Entity* canvas, std::vector<Paddle> paddle_list, std::vector<Barrier> barrier_list, const float& delta_time)
{

	for(Paddle& temp_paddle : paddle_list)
	{
		SDL_Rect border_box = Entity::get_border_box();
		SDL_Rect pad_border_box = temp_paddle.get_border_box();

		// ball corners: top_left, top_right, bottom_right, bottom_left
		std::vector<Vector2f> ball_corners{
			Vector2f(border_box.x + velocity.x, border_box.y + velocity.y),
			Vector2f(border_box.x + border_box.w + velocity.x, border_box.y + velocity.y),
			Vector2f(border_box.x + border_box.w + velocity.x, border_box.y + border_box.h + velocity.y),
			Vector2f(border_box.x + velocity.x, border_box.y + border_box.h + velocity.y)
		};

		// paddle corners
		Vector2f top_left(pad_border_box.x, pad_border_box.y);
		Vector2f top_right(pad_border_box.x + pad_border_box.w, pad_border_box.y);
		Vector2f bottom_right(pad_border_box.x + pad_border_box.w, pad_border_box.y + pad_border_box.h);
		Vector2f bottom_left(pad_border_box.x, pad_border_box.y + pad_border_box.h);

		// sub-triangles of the rectangle (paddle's border box) - splits it like an X (for doing math later)
		Triangle top(temp_paddle.get_center(), top_left, top_right);
		Triangle right(temp_paddle.get_center(), top_right, bottom_right);
		Triangle bottom(temp_paddle.get_center(), bottom_right, bottom_left);
		Triangle left(temp_paddle.get_center(), bottom_left, top_left);


		// top, right, bottom, left
		bool intersect_1 = false;
		bool intersect_2 = false;
		bool intersect_3 = false;
		bool intersect_4 = false;

		//may have to adjust for next turn? not sure
		for(Vector2f corner : ball_corners)
		{
			if(!intersect_1 && top.is_point_within(corner))
				intersect_1 = true;
			if(!intersect_2 && right.is_point_within(corner))
				intersect_2 = true;
			if(!intersect_3 && bottom.is_point_within(corner))
				intersect_3 = true;
			if(!intersect_4 && left.is_point_within(corner))
				intersect_4 = true;
		}

		//unlikely outlier cases, but necessary for if some paddle resize tomfoolery occurs
		if(intersect_1 && intersect_2 && intersect_3)
		{
			//bounces off right
			Entity::set_pos(Vector2f(pad_border_box.x + pad_border_box.w + border_box.w/2, get_pos().y));
			scale_velocity(Vector2f(-1, 1));
			random_rotation_velocity();
		}
		else if(intersect_2 && intersect_3 && intersect_4)
		{
			//bounces off bottom
			Entity::set_pos(Vector2f(get_pos().x, pad_border_box.y + pad_border_box.h + border_box.h/2));
			scale_velocity(Vector2f(1, -1));
			random_rotation_velocity();
		}
		else if(intersect_3 && intersect_4 && intersect_1)
		{
			//bounces off left
			Entity::set_pos(Vector2f(pad_border_box.x - border_box.w/2, get_pos().y));
			scale_velocity(Vector2f(-1, 1));
			random_rotation_velocity();
		}
		else if(intersect_4 && intersect_1 && intersect_2)
		{
			//bounces off top
			Entity::set_pos(Vector2f(get_pos().x, pad_border_box.y - border_box.h/2));
			scale_velocity(Vector2f(1, -1));
			random_rotation_velocity();
		}
		else if(intersect_1 && intersect_3)
		{
			bool bounce_off_top = get_pos().y < temp_paddle.get_center().y;
			if(bounce_off_top)
			{
				Entity::set_pos(Vector2f(get_pos().x, pad_border_box.y - border_box.h/2));
			}
			else
			{
				Entity::set_pos(Vector2f(get_pos().x, pad_border_box.y + pad_border_box.h + border_box.h/2));
			}
			scale_velocity(Vector2f(1, -1));
			random_rotation_velocity();
		}
		else if(intersect_2 && intersect_4)
		{
			bool bounce_off_left = get_pos().x < temp_paddle.get_center().x;
			if(bounce_off_left)
			{
				Entity::set_pos(Vector2f(pad_border_box.x - border_box.w/2, get_pos().y));
			}
			else
			{
				Entity::set_pos(Vector2f(pad_border_box.x + pad_border_box.w + border_box.w/2, get_pos().y));
			}
			scale_velocity(Vector2f(-1, 1));
			random_rotation_velocity();
		}

		//corner cases
		else if(intersect_1 && intersect_2)
		{
			float distance1 = game_math::distance(top_right, ball_corners[0]);
			float distance2 = game_math::distance(top_right, ball_corners[2]);

			//perfectly hitting the corner
			if(distance1 == distance2)
			{
				Entity::set_pos(Vector2f(pad_border_box.x + pad_border_box.w + border_box.w/2, pad_border_box.y - border_box.h/2));
				set_velocity_dir(Vector2f(1, -1));
			}
			//hitting the top side
			else if(distance1 > distance2)
			{
				Entity::set_pos(Vector2f(get_pos().x, pad_border_box.y - border_box.h/2));
				scale_velocity(Vector2f(1, -1));
			}
			//hitting the right side
			else
			{
				Entity::set_pos(Vector2f(pad_border_box.x + pad_border_box.w + border_box.w/2, get_pos().y));
				scale_velocity(Vector2f(-1, 1));
			}
			random_rotation_velocity();
		}
		else if(intersect_2 && intersect_3)
		{
			float distance1 = game_math::distance(bottom_right, ball_corners[1]);
			float distance2 = game_math::distance(bottom_right, ball_corners[3]);

			//perfectly hitting the corner
			if(distance1 == distance2)
			{
				Entity::set_pos(Vector2f(pad_border_box.x + pad_border_box.w + border_box.w/2, pad_border_box.y + pad_border_box.h + border_box.h/2));
				set_velocity_dir(Vector2f(1, 1));
			}
			//hitting the right side
			else if(distance1 > distance2)
			{
				Entity::set_pos(Vector2f(pad_border_box.x + pad_border_box.w + border_box.w/2, get_pos().y));
				scale_velocity(Vector2f(-1, 1));
			}
			//hitting the bottom side
			else
			{
				Entity::set_pos(Vector2f(get_pos().x, pad_border_box.y + pad_border_box.h + border_box.h/2));
				scale_velocity(Vector2f(1, -1));
			}
			random_rotation_velocity();
		}
		else if(intersect_3 && intersect_4)
		{
			float distance1 = game_math::distance(bottom_left, ball_corners[0]);
			float distance2 = game_math::distance(bottom_left, ball_corners[2]);

			//perfectly hitting the corner
			if(distance1 == distance2)
			{
				Entity::set_pos(Vector2f(pad_border_box.x - border_box.w/2, pad_border_box.y + pad_border_box.h + border_box.h/2));
				set_velocity_dir(Vector2f(-1, 1));
			}
			//hitting the left side
			else if(distance1 > distance2)
			{
				Entity::set_pos(Vector2f(pad_border_box.x - border_box.w/2, get_pos().y));
				scale_velocity(Vector2f(-1, 1));
			}
			//hitting the bottom side
			else
			{
				Entity::set_pos(Vector2f(get_pos().x, pad_border_box.y + pad_border_box.h + border_box.h/2));
				scale_velocity(Vector2f(1, -1));
			}
			random_rotation_velocity();
		}
		else if(intersect_4 && intersect_1)
		{
			float distance1 = game_math::distance(top_left, ball_corners[1]);
			float distance2 = game_math::distance(top_left, ball_corners[3]);

			//perfectly hitting the corner
			if(distance1 == distance2)
			{
				Entity::set_pos(Vector2f(pad_border_box.x - border_box.w/2, pad_border_box.y - border_box.h/2));
				set_velocity_dir(Vector2f(-1, -1));
			}
			//hitting the top side
			else if(distance1 > distance2)
			{
				Entity::set_pos(Vector2f(get_pos().x, pad_border_box.y - border_box.h/2));
				scale_velocity(Vector2f(1, -1));
			}
			//hitting the left side
			else
			{
				Entity::set_pos(Vector2f(pad_border_box.x - border_box.w/2, get_pos().y));
				scale_velocity(Vector2f(-1, 1));
			}
			random_rotation_velocity();
		}

		//regular edge cases
		else if(intersect_1 || (intersect_2 && velocity.x > 0 && velocity.y > 0) || (intersect_4 && velocity.x < 0 && velocity.y > 0))
		{
			Entity::set_pos(Vector2f(get_pos().x, pad_border_box.y - border_box.h/2));
			scale_velocity(Vector2f(1, -1));

			random_rotation_velocity();
		}
		else if(intersect_2 || (intersect_1 && velocity.x < 0 && velocity.y < 0) || (intersect_3 && velocity.x < 0 && velocity.y > 0))
		{
			Entity::set_pos(Vector2f(
				pad_border_box.x + pad_border_box.w + border_box.w/2,
				get_pos().y
			));
			scale_velocity(Vector2f(-1, 1));

			float percent_from_center = 100 * game_math::clamp(2 * abs(temp_paddle.get_center().y - get_pos().y) / temp_paddle.get_border_box().h, 0, 1);
			set_rotation_direction(1);
			random_rotation_velocity();
			bounce_y_velocity(percent_from_center);
		}
		else if(intersect_3 || (intersect_2 && velocity.x > 0 && velocity.y < 0) || (intersect_4 && velocity.x < 0 && velocity.y < 0))
		{
			Entity::set_pos(Vector2f(get_pos().x, pad_border_box.y + pad_border_box.h + border_box.h/2));
			scale_velocity(Vector2f(1, -1));

			random_rotation_velocity();
		}
		else if(intersect_4 || (intersect_1 && velocity.x > 0 && velocity.y < 0) || (intersect_3 && velocity.x > 0 && velocity.y > 0))
		{
			Entity::set_pos(Vector2f(pad_border_box.x - border_box.w/2, get_pos().y));
			scale_velocity(Vector2f(-1, 1));

			float percent_from_center = 100 * game_math::clamp(2 * abs(temp_paddle.get_center().y - get_pos().y) / temp_paddle.get_border_box().h, 0, 1);
			set_rotation_direction(0);
			random_rotation_velocity();
			bounce_y_velocity(percent_from_center);
		}

	}


	for(Barrier& temp_barrier : barrier_list)
	{
		SDL_Rect border_box = Entity::get_border_box();
		SDL_Rect bar_border_box = temp_barrier.get_border_box();

		// ball corners: top_left, top_right, bottom_right, bottom_left
		std::vector<Vector2f> ball_corners{
			Vector2f(border_box.x + velocity.x, border_box.y + velocity.y),
			Vector2f(border_box.x + border_box.w + velocity.x, border_box.y + velocity.y),
			Vector2f(border_box.x + border_box.w + velocity.x, border_box.y + border_box.h + velocity.y),
			Vector2f(border_box.x + velocity.x, border_box.y + border_box.h + velocity.y)
		};

		// Barrier corners
		Vector2f top_left(bar_border_box.x, bar_border_box.y);
		Vector2f top_right(bar_border_box.x + bar_border_box.w, bar_border_box.y);
		Vector2f bottom_right(bar_border_box.x + bar_border_box.w, bar_border_box.y + bar_border_box.h);
		Vector2f bottom_left(bar_border_box.x, bar_border_box.y + bar_border_box.h);

		// sub-triangles of the rectangle (Barrier's border box) - splits it like an X (for doing math later)
		Triangle top(temp_barrier.get_center(), top_left, top_right);
		Triangle right(temp_barrier.get_center(), top_right, bottom_right);
		Triangle bottom(temp_barrier.get_center(), bottom_right, bottom_left);
		Triangle left(temp_barrier.get_center(), bottom_left, top_left);


		// top, right, bottom, left
		bool intersect_1 = false;
		bool intersect_2 = false;
		bool intersect_3 = false;
		bool intersect_4 = false;

		//may have to adjust for next turn? not sure
		for(Vector2f corner : ball_corners)
		{
			if(!intersect_1 && top.is_point_within(corner))
				intersect_1 = true;
			if(!intersect_2 && right.is_point_within(corner))
				intersect_2 = true;
			if(!intersect_3 && bottom.is_point_within(corner))
				intersect_3 = true;
			if(!intersect_4 && left.is_point_within(corner))
				intersect_4 = true;
		}

		//unlikely outlier cases, but necessary for if some barrier resize tomfoolery occurs
		if(intersect_1 && intersect_2 && intersect_3)
		{
			//bounces off right
			Entity::set_pos(Vector2f(bar_border_box.x + bar_border_box.w + border_box.w/2, get_pos().y));
			scale_velocity(Vector2f(-1, 1));
			random_rotation_velocity();
		}
		else if(intersect_2 && intersect_3 && intersect_4)
		{
			//bounces off bottom
			Entity::set_pos(Vector2f(get_pos().x, bar_border_box.y + bar_border_box.h + border_box.h/2));
			scale_velocity(Vector2f(1, -1));
			random_rotation_velocity();
		}
		else if(intersect_3 && intersect_4 && intersect_1)
		{
			//bounces off left
			Entity::set_pos(Vector2f(bar_border_box.x - border_box.w/2, get_pos().y));
			scale_velocity(Vector2f(-1, 1));
			random_rotation_velocity();
		}
		else if(intersect_4 && intersect_1 && intersect_2)
		{
			//bounces off top
			Entity::set_pos(Vector2f(get_pos().x, bar_border_box.y - border_box.h/2));
			scale_velocity(Vector2f(1, -1));
			random_rotation_velocity();
		}
		else if(intersect_1 && intersect_3)
		{
			bool bounce_off_top = get_pos().y < temp_barrier.get_center().y;
			if(bounce_off_top)
			{
				Entity::set_pos(Vector2f(get_pos().x, bar_border_box.y - border_box.h/2));
			}
			else
			{
				Entity::set_pos(Vector2f(get_pos().x, bar_border_box.y + bar_border_box.h + border_box.h/2));
			}
			scale_velocity(Vector2f(1, -1));
			random_rotation_velocity();
		}
		else if(intersect_2 && intersect_4)
		{
			bool bounce_off_left = get_pos().x < temp_barrier.get_center().x;
			if(bounce_off_left)
			{
				Entity::set_pos(Vector2f(bar_border_box.x - border_box.w/2, get_pos().y));
			}
			else
			{
				Entity::set_pos(Vector2f(bar_border_box.x + bar_border_box.w + border_box.w/2, get_pos().y));
			}
			scale_velocity(Vector2f(-1, 1));
			random_rotation_velocity();
		}

		//corner cases
		else if(intersect_1 && intersect_2)
		{
			float distance1 = game_math::distance(top_right, ball_corners[0]);
			float distance2 = game_math::distance(top_right, ball_corners[2]);

			//perfectly hitting the corner
			if(distance1 == distance2)
			{
				Entity::set_pos(Vector2f(bar_border_box.x + bar_border_box.w + border_box.w/2, bar_border_box.y - border_box.h/2));
				set_velocity_dir(Vector2f(1, -1));
			}
			//hitting the top side
			else if(distance1 > distance2)
			{
				Entity::set_pos(Vector2f(get_pos().x, bar_border_box.y - border_box.h/2));
				scale_velocity(Vector2f(1, -1));
			}
			//hitting the right side
			else
			{
				Entity::set_pos(Vector2f(bar_border_box.x + bar_border_box.w + border_box.w/2, get_pos().y));
				scale_velocity(Vector2f(-1, 1));
			}
			random_rotation_velocity();
		}
		else if(intersect_2 && intersect_3)
		{
			float distance1 = game_math::distance(bottom_right, ball_corners[1]);
			float distance2 = game_math::distance(bottom_right, ball_corners[3]);

			//perfectly hitting the corner
			if(distance1 == distance2)
			{
				Entity::set_pos(Vector2f(bar_border_box.x + bar_border_box.w + border_box.w/2, bar_border_box.y + bar_border_box.h + border_box.h/2));
				set_velocity_dir(Vector2f(1, 1));
			}
			//hitting the right side
			else if(distance1 > distance2)
			{
				Entity::set_pos(Vector2f(bar_border_box.x + bar_border_box.w + border_box.w/2, get_pos().y));
				scale_velocity(Vector2f(-1, 1));
			}
			//hitting the bottom side
			else
			{
				Entity::set_pos(Vector2f(get_pos().x, bar_border_box.y + bar_border_box.h + border_box.h/2));
				scale_velocity(Vector2f(1, -1));
			}
			random_rotation_velocity();
		}
		else if(intersect_3 && intersect_4)
		{
			float distance1 = game_math::distance(bottom_left, ball_corners[0]);
			float distance2 = game_math::distance(bottom_left, ball_corners[2]);

			//perfectly hitting the corner
			if(distance1 == distance2)
			{
				Entity::set_pos(Vector2f(bar_border_box.x - border_box.w/2, bar_border_box.y + bar_border_box.h + border_box.h/2));
				set_velocity_dir(Vector2f(-1, 1));
			}
			//hitting the left side
			else if(distance1 > distance2)
			{
				Entity::set_pos(Vector2f(bar_border_box.x - border_box.w/2, get_pos().y));
				scale_velocity(Vector2f(-1, 1));
			}
			//hitting the bottom side
			else
			{
				Entity::set_pos(Vector2f(get_pos().x, bar_border_box.y + bar_border_box.h + border_box.h/2));
				scale_velocity(Vector2f(1, -1));
			}
			random_rotation_velocity();
		}
		else if(intersect_4 && intersect_1)
		{
			float distance1 = game_math::distance(top_left, ball_corners[1]);
			float distance2 = game_math::distance(top_left, ball_corners[3]);

			//perfectly hitting the corner
			if(distance1 == distance2)
			{
				Entity::set_pos(Vector2f(bar_border_box.x - border_box.w/2, bar_border_box.y - border_box.h/2));
				set_velocity_dir(Vector2f(-1, -1));
			}
			//hitting the top side
			else if(distance1 > distance2)
			{
				Entity::set_pos(Vector2f(get_pos().x, bar_border_box.y - border_box.h/2));
				scale_velocity(Vector2f(1, -1));
			}
			//hitting the left side
			else
			{
				Entity::set_pos(Vector2f(bar_border_box.x - border_box.w/2, get_pos().y));
				scale_velocity(Vector2f(-1, 1));
			}
			random_rotation_velocity();
		}

		//regular edge cases
		else if(intersect_1 || (intersect_2 && velocity.x > 0 && velocity.y > 0) || (intersect_4 && velocity.x < 0 && velocity.y > 0))
		{
			Entity::set_pos(Vector2f(get_pos().x, bar_border_box.y - border_box.h/2));
			scale_velocity(Vector2f(1, -1));

			random_rotation_velocity();
		}
		else if(intersect_2 || (intersect_1 && velocity.x < 0 && velocity.y < 0) || (intersect_3 && velocity.x < 0 && velocity.y > 0))
		{
			Entity::set_pos(Vector2f(
				bar_border_box.x + bar_border_box.w + border_box.w/2,
				get_pos().y
			));
			scale_velocity(Vector2f(-1, 1));

			float percent_from_center = 100 * game_math::clamp(2 * abs(temp_barrier.get_center().y - get_pos().y) / temp_barrier.get_border_box().h, 0, 1);
			set_rotation_direction(1);
			random_rotation_velocity();
			bounce_y_velocity(percent_from_center);
		}
		else if(intersect_3 || (intersect_2 && velocity.x > 0 && velocity.y < 0) || (intersect_4 && velocity.x < 0 && velocity.y < 0))
		{
			Entity::set_pos(Vector2f(get_pos().x, bar_border_box.y + bar_border_box.h + border_box.h/2));
			scale_velocity(Vector2f(1, -1));

			random_rotation_velocity();
		}
		else if(intersect_4 || (intersect_1 && velocity.x > 0 && velocity.y < 0) || (intersect_3 && velocity.x > 0 && velocity.y > 0))
		{
			Entity::set_pos(Vector2f(bar_border_box.x - border_box.w/2, get_pos().y));
			scale_velocity(Vector2f(-1, 1));

			float percent_from_center = 100 * game_math::clamp(2 * abs(temp_barrier.get_center().y - get_pos().y) / temp_barrier.get_border_box().h, 0, 1);
			set_rotation_direction(0);
			random_rotation_velocity();
			bounce_y_velocity(percent_from_center);
		}

	}

//canvas
	SDL_Rect border_box = Entity::get_border_box();

	// ball corners: top_left, top_right, bottom_right, bottom_left
	std::vector<Vector2f> ball_corners{
		Vector2f(border_box.x + velocity.x, border_box.y + velocity.y),
		Vector2f(border_box.x + border_box.w + velocity.x, border_box.y + velocity.y),
		Vector2f(border_box.x + border_box.w + velocity.x, border_box.y + border_box.h + velocity.y),
		Vector2f(border_box.x + velocity.x, border_box.y + border_box.h + velocity.y)
	};

	bool top_left_canvas, top_right_canvas, bottom_right_canvas, bottom_left_canvas = false;

	if(canvas->is_point_within(ball_corners[0]))
		top_left_canvas = true;
	if(canvas->is_point_within(ball_corners[1]))
		top_right_canvas = true;
	if(canvas->is_point_within(ball_corners[2]))
		bottom_right_canvas = true;
	if(canvas->is_point_within(ball_corners[3]))
		bottom_left_canvas = true;

	GOAL_SCORE goal_scored = NO_SCORE;

	if(!top_left_canvas && !bottom_left_canvas)
	{
		Entity::set_pos(Vector2f(border_box.w/2, get_pos().y));
		scale_velocity(Vector2f(-1, 1));
		goal_scored = P2_SCORE;
	}
	else if(!top_right_canvas && !bottom_right_canvas)
	{
		Entity::set_pos(Vector2f(utils::display_width() - border_box.w/2, get_pos().y));
		scale_velocity(Vector2f(-1, 1));
		goal_scored = P1_SCORE;
	}
	if(!top_left_canvas && !top_right_canvas)
	{
		Entity::set_pos(Vector2f(get_pos().x, border_box.h/2));
		scale_velocity(Vector2f(1, -1));
	}	
	else if(!bottom_left_canvas && !bottom_right_canvas)
	{
		Entity::set_pos(Vector2f(get_pos().x, utils::display_height() - border_box.h/2));
		scale_velocity(Vector2f(1, -1));
	}


	Entity::rotate_deg(rotation_velocity);
	
	Entity::set_pos(next_pos());

	return goal_scored;

}