#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_ttf.h>
#include <vector>
#include <string>

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