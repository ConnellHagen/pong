#include <ctime>

#include "Math.hpp"
#include "Entity.hpp"
#include "Ball.hpp"

Ball::Ball(Vector2f const &p_pos, Vector2f const &p_scale, SDL_Texture* const &p_texture, SDL_Rect const &p_imgdata, const int& p_render_mode)
	:Entity(p_pos, p_scale, p_texture, p_imgdata, p_render_mode)
{
	Entity::set_original_image(p_imgdata);
	velocity = Vector2f(3, 3);
}

void Ball::scale_velocity(Vector2f const &p_scale)
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
void Ball::set_rotation_direction(int const &direction)
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