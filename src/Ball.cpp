#include "Math.hpp"
#include "Entity.hpp"
#include "Ball.hpp"

Ball::Ball(Vector2f const &p_pos, Vector2f const &p_scale, SDL_Texture* const &p_texture, SDL_Rect const &p_imgdata)
	:Entity(p_pos, p_scale, p_texture, p_imgdata)
{
	Entity::set_original_image(p_imgdata);
	velocity = Vector2f(3, 3);
}

void Ball::scale_velocity(Vector2f const &p_scale)
{
	velocity.x *= p_scale.x;
	velocity.y *= p_scale.y;
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
	Entity::rotate_deg(1.0f);
	Entity::set_pos(next_pos());
}