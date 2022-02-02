#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>

#include "Math.hpp"

#include "Entity.hpp"
	#include "Barrier.hpp"

Barrier::Barrier(const Vector2f& p_pos, const Vector2f& p_scale, SDL_Texture* p_texture, const SDL_Rect& p_sheet, const SDL_Rect& p_current, const int& p_render_mode)
	:Entity(p_pos, p_scale, p_texture, p_sheet, p_current, p_render_mode)
{
}

void Barrier::update(const float& delta_time)
{
	frame_timer += delta_time;

	if(frame_timer >= .125)
	{
		Entity::next_sprite_frame();
		frame_timer -= .125;
	}
		
}