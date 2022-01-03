#include <ctime>
#include <cmath>

#include "Math.hpp"
#include "Entity.hpp"
#include "Ball.hpp"

Ball::Ball(const Vector2f& p_pos, const Vector2f& p_scale, SDL_Texture* p_texture, const SDL_Rect& p_sheet, const SDL_Rect& p_current, const int& p_render_mode)
	:Entity(p_pos, p_scale, p_texture, p_sheet, p_current, p_render_mode), net_velocity(6)
{
	velocity = Vector2f(net_velocity, 0);
}

void Ball::bounce_y_velocity(const float& p_percent_off)
{
	float percent_off = p_percent_off;
	float y_vel = ceil(percent_off / 10.0) / -2.0f;
	velocity.y = y_vel;

	update_x_velocity();
}

void Ball::scale_velocity(const Vector2f& p_scale)
{
	velocity.x *= p_scale.x;
	velocity.y *= p_scale.y;
}

void Ball::random_rotation_velocity()
{
	bool is_negative = rotation_velocity < 0;
	srand((unsigned int) time (NULL));
    rotation_velocity = rand() % 7 + 1;
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

void Ball::update()
{
	Entity::rotate_deg(rotation_velocity);
	Entity::set_pos(next_pos());
}

void Ball::update_x_velocity()
{
	bool is_negative = velocity.x < 0;
	float x_squared = pow(net_velocity, 2) - pow(velocity.y, 2);
	float x = sqrt(x_squared);
	if(is_negative)
		x *= -1;
	velocity.x = x;
}