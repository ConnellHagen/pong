#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_ttf.h>
#include <vector>
#include <string>
#include <cmath>

#include "Text.hpp"
#include "Math.hpp"

Text::Text(SDL_Renderer* p_renderer, const int& p_render_mode, const std::string& p_font_path, const int& p_size, const SDL_Color& p_color, const std::string& p_text, const Vector2f& p_pos)
	:renderer(p_renderer), render_mode(p_render_mode), font_path(p_font_path), size(p_size), color(p_color), text(p_text), pos(p_pos)
{
	font = TTF_OpenFont(font_path.c_str(), size);

	SDL_Surface* surface = TTF_RenderText_Solid(font, text.c_str(), color);

	original_box = SDL_Rect{static_cast<int>(pos.x), static_cast<int>(pos.y), surface->w, surface->h};
	build_border_box();

	texture = SDL_CreateTextureFromSurface(renderer, surface);

	SDL_FreeSurface(surface);
}
	
Text::~Text()
{
	// SDL_DestroyTexture(texture);
	// TTF_CloseFont(font);
}

void Text::resize_font(const int& p_size)
{
	size = p_size;

	SDL_DestroyTexture(texture);
	TTF_CloseFont(font);

	font = TTF_OpenFont(font_path.c_str(), size);

	SDL_Surface* surface = TTF_RenderText_Solid(font, text.c_str(), color);

	original_box = SDL_Rect{static_cast<int>(pos.x), static_cast<int>(pos.y), surface->w, surface->h};
	build_border_box();

	texture = SDL_CreateTextureFromSurface(renderer, surface);

	SDL_FreeSurface(surface);
}

void Text::recolor_font(const SDL_Color& p_color)
{
	color = p_color;
	
	SDL_DestroyTexture(texture);

	SDL_Surface* surface = TTF_RenderText_Solid(font, text.c_str(), color);

	texture = SDL_CreateTextureFromSurface(renderer, surface);

	SDL_FreeSurface(surface);
}

void Text::change_text(const std::string& p_text)
{
	text = p_text;

	SDL_DestroyTexture(texture);
	TTF_CloseFont(font);

	font = TTF_OpenFont(font_path.c_str(), size);

	SDL_Surface* surface = TTF_RenderText_Solid(font, text.c_str(), color);

	original_box = SDL_Rect{static_cast<int>(pos.x), static_cast<int>(pos.y), surface->w, surface->h};
	build_border_box();

	texture = SDL_CreateTextureFromSurface(renderer, surface);

	SDL_FreeSurface(surface);

}

void Text::change_render_mode(const int& p_render_mode)
{
	render_mode = p_render_mode;

	build_border_box();
}


void Text::build_border_box()
{
	SDL_Rect new_border = original_box;

	switch(render_mode)
	{
	case 1:
		break;
	case 2:
		new_border.x -= original_box.w/2;
		break;
	case 3:
		new_border.x -= original_box.w;
		break;
	case 4:
		new_border.y -= original_box.h/2;
		break;
	case 5:
		new_border.x -= original_box.w/2;
		new_border.y -= original_box.h/2;
		break;
	case 6:
		new_border.x -= original_box.w;
		new_border.y -= original_box.h/2;
		break;
	case 7:
		new_border.y -= original_box.h;
		break;
	case 8:
		new_border.x -= original_box.w/2;
		new_border.y -= original_box.h;
		break;
	case 9:
		new_border.x -= original_box.w;
		new_border.y -= original_box.h;
		break;
	}

	border_box = new_border;
}

TextButton::TextButton(const Text& p_text, const SDL_Color& p_hover_color, const BUTTON_FUNCTION& p_press_function)
	:text(p_text), current_state(UNPRESSED), default_color(p_text.color), hover_color(p_hover_color), press_color(SDL_Color{255, 255, 255, 255})
{}

TextButton::TextButton(const Text& p_text, const SDL_Color& p_hover_color, const SDL_Color& p_press_color, const BUTTON_FUNCTION& p_press_function)
	:text(p_text), current_state(UNPRESSED), default_color(p_text.color), hover_color(p_hover_color), press_color(p_press_color), press_function(p_press_function)
{}

TextButton::~TextButton()
{}

std::vector<BUTTON_FUNCTION> TextButton::update(const std::vector<bool>& key_pushes, const Vector2i& mouse_coords)
{
	std::vector<BUTTON_FUNCTION> functions;

	color_state new_state = current_state;

	// checks if the button has changed its state in the last frame
	if(new_state != PRESSED && key_pushes[4] && game_math::contains_point(get_border_box(), mouse_coords))
	{
		new_state = PRESSED;
	}
	else if(new_state == PRESSED && !key_pushes[4])
	{
		new_state = game_math::contains_point(get_border_box(), mouse_coords)? HOVER : UNPRESSED;
	}
	else if(new_state != PRESSED)
	{
		new_state = game_math::contains_point(get_border_box(), mouse_coords)? HOVER : UNPRESSED;
	}
	
	// if it has changed states, then makes the necessary changes
	// first block of code is for if the button is being held/released, activating the co-responding function
	// the second is for changing the color if it isn't being pressed
	if(new_state == PRESSED && current_state != PRESSED) // pressed button
	{
		text.recolor_font(press_color);
		current_state = PRESSED;
	}
	else if(new_state != PRESSED && current_state == PRESSED) // released button
	{
		functions.push_back(press_function);
	}

	if(new_state != PRESSED)
	{
		if(new_state == HOVER)
		{
			text.recolor_font(hover_color);
			current_state = HOVER;
		}
		else
		{
			text.recolor_font(default_color);
			current_state = UNPRESSED;
		}
	}

	return functions;
}

TextImage::TextImage(const Vector2f& p_pos, const Vector2f& p_scale, SDL_Texture* p_texture, const SDL_Rect& p_sheet, const SDL_Rect& p_current, const int& p_render_mode, const MOVEMENT_TYPE& p_movement, const int& p_magnitude)
	:sprite_sheet(p_sheet), current_sprite_frame(p_current), texture(p_texture), original_pos(p_pos), pos(p_pos), scale(p_scale), movement(p_movement), magnitude(p_magnitude), render_mode(p_render_mode)
{
	build_border_box();
}

std::vector<BUTTON_FUNCTION> TextImage::update(const float& delta_time)
{
	std::vector<BUTTON_FUNCTION> functions;

	accumulator += delta_time;
	move();

	return functions;
}


void TextImage::move()
{
	switch(movement)
	{
	case SINE:
		pos.y = original_pos.y + magnitude * std::sin(accumulator);
		build_border_box();
		break;

	default:
		break;
	}
}

void TextImage::build_border_box()
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