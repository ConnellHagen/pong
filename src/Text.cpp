#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_ttf.h>
#include <vector>
#include <string>

#include "Text.hpp"
#include "Math.hpp"

Text::Text(SDL_Renderer* p_renderer, const std::string& p_font_path, const int& p_size, const SDL_Color& p_color, const std::string& p_text, const Vector2f& p_pos)
	:renderer(p_renderer), font_path(p_font_path), size(p_size), color(p_color), text(p_text), pos(p_pos)
{
	font = TTF_OpenFont(font_path.c_str(), size);

	SDL_Surface* surface = TTF_RenderText_Solid(font, text.c_str(), color);

	border_box = SDL_Rect{static_cast<int>(pos.x), static_cast<int>(pos.y), surface->w, surface->h};
	src = SDL_Rect{0, 0, surface->w, surface->h};

	texture = SDL_CreateTextureFromSurface(renderer, surface);

	SDL_FreeSurface(surface);
}
	
Text::~Text()
{
	TTF_CloseFont(font);
}