#include "Entity.hpp"

SDL_Rect Entity::get_border_box()
{
	SDL_Rect temp_rect = get_original_image();
	temp_rect.x = get_pos().x;
	temp_rect.y = get_pos().y;
	temp_rect.w = temp_rect.w * get_scale().x;
	temp_rect.h = temp_rect.h * get_scale().y;
	return temp_rect;
}

bool Entity::is_point_within(Vector2f const &point)
{
	if(!(point.x >= pos.x && point.x < pos.x + scale.x * original_img.w))
		return false;

	if(!(point.y >= pos.y && point.y < pos.y + scale.y * original_img.h))
		return false;

	return true;
}

