#include "Entity.hpp"

Entity::Entity(Vector2f const &p_pos, Vector2f const &p_scale, SDL_Texture* const &p_texture, SDL_Rect p_imgdata, const int& p_render_mode)
	:original_img(p_imgdata), texture(p_texture), pos(p_pos), scale(p_scale), render_mode(p_render_mode)
{
	build_border_box();
}

void Entity::set_pos(Vector2f const &p_pos)
{
	pos = p_pos;
	build_border_box();
}

void Entity::set_scale(float const &p_w, float const &p_h)
{
	scale = Vector2f(p_w, p_h);
	build_border_box();
}


void Entity::build_border_box()
{
	SDL_Rect new_border = original_img;
	new_border.x = pos.x;
	new_border.y = pos.y;
	new_border.w *= scale.x;
	new_border.h *= scale.y;

	switch(render_mode)
	{
		case 1:
			break;
		case 2:
			new_border.x -= original_img.w/2 * scale.x;
			break;
		case 3:
			new_border.x -= original_img.w * scale.x;
			break;
		case 4:
			new_border.y -= original_img.h/2 * scale.y;
			break;
		case 5:
			new_border.x -= original_img.w/2 * scale.x;
			new_border.y -= original_img.h/2 * scale.y;
			break;
		case 6:
			new_border.x -= original_img.w * scale.x;
			new_border.y -= original_img.h/2 * scale.y;
			break;
		case 7:
			new_border.y -= original_img.h * scale.y;
			break;
		case 8:
			new_border.x -= original_img.w/2 * scale.x;
			new_border.y -= original_img.h * scale.y;
			break;
		case 9:
			new_border.x -= original_img.w * scale.x;
			new_border.y -= original_img.h * scale.y;
			break;
	}

	border_box = new_border;
}

bool Entity::is_point_within(Vector2f const &point)
{
	if(!(point.x >= border_box.x && point.x < border_box.x + border_box.w))
		return false;

	if(!(point.y >= border_box.y && point.y < border_box.y + border_box.h))
		return false;

	return true;
}

