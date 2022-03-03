#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <iostream>

#include "Math.hpp"
#include "Entity.hpp"

Entity::Entity(const Vector2f& p_pos, const Vector2f& p_scale, SDL_Texture* p_texture, const SDL_Rect& p_sheet, const SDL_Rect& p_current, const int& p_render_mode)
	:sprite_sheet(p_sheet), current_sprite_frame(p_current), texture(p_texture), pos(p_pos), scale(p_scale), render_mode(p_render_mode)
{
	build_border_box();
}

void Entity::set_pos(const Vector2f& p_pos)
{
	pos = p_pos;
	build_border_box();
}

Vector2f Entity::get_center()
{
	float temp_x = border_box.x + border_box.w/2;
	float temp_y = border_box.y + border_box.h/2;
	return Vector2f(temp_x, temp_y);
}

void Entity::set_scale(const float& p_w, const float& p_h)
{
	scale = Vector2f(p_w, p_h);
	build_border_box();
}


void Entity::build_border_box()
{
	SDL_Rect new_border = current_sprite_frame;
	new_border.x = pos.x;
	new_border.y = pos.y;
	new_border.w *= scale.x;
	new_border.h *= scale.y;

	switch(render_mode)
	{
	case 1:
		break;
	case 2:
		new_border.x -= current_sprite_frame.w/2 * scale.x;
		break;
	case 3:
		new_border.x -= current_sprite_frame.w * scale.x;
		break;
	case 4:
		new_border.y -= current_sprite_frame.h/2 * scale.y;
		break;
	case 5:
		new_border.x -= current_sprite_frame.w/2 * scale.x;
		new_border.y -= current_sprite_frame.h/2 * scale.y;
		break;
	case 6:
		new_border.x -= current_sprite_frame.w * scale.x;
		new_border.y -= current_sprite_frame.h/2 * scale.y;
		break;
	case 7:
		new_border.y -= current_sprite_frame.h * scale.y;
		break;
	case 8:
		new_border.x -= current_sprite_frame.w/2 * scale.x;
		new_border.y -= current_sprite_frame.h * scale.y;
		break;
	case 9:
		new_border.x -= current_sprite_frame.w * scale.x;
		new_border.y -= current_sprite_frame.h * scale.y;
		break;
	}

	border_box = new_border;
}

void Entity::next_sprite_frame()
{
	if(current_sprite_frame.x + current_sprite_frame.h == sprite_sheet.w)
	{
		if(current_sprite_frame.y + current_sprite_frame.h == sprite_sheet.h)
		{
			current_sprite_frame.x = 0;
			current_sprite_frame.y = 0;
			return;
		}
		current_sprite_frame.x = 0;
		current_sprite_frame.y += current_sprite_frame.h;
		return;
	}
	current_sprite_frame.x += current_sprite_frame.w;
}

bool Entity::is_point_within(const Vector2f& point)
{
	if(!(point.x >= border_box.x && point.x < border_box.x + border_box.w))
		return false;

	if(!(point.y >= border_box.y && point.y < border_box.y + border_box.h))
		return false;

	return true;
}

