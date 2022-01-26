#pragma once

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_ttf.h>
#include <vector>
#include <string>

#include "Math.hpp"

struct Text
{
	Text(SDL_Renderer* p_renderer, const std::string& p_font_path, const int& p_size, const SDL_Color& p_color, const std::string& p_text, const Vector2f& p_pos);
	~Text();

	void resize_font(const int& p_size);
	void recolor_font(const SDL_Color& p_color);


	SDL_Renderer* renderer;

	std::string font_path;
	int size;
	TTF_Font* font;

	SDL_Color color;

	std::string text;

	Vector2f pos;
	SDL_Rect border_box;
	SDL_Rect src;

	SDL_Texture* texture;

};