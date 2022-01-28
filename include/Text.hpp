#pragma once

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_ttf.h>
#include <vector>
#include <string>

#include "Math.hpp"

struct Text
{
public:
	Text(SDL_Renderer* p_renderer, const int& p_render_mode, const std::string& p_font_path, const int& p_size, const SDL_Color& p_color, const std::string& p_text, const Vector2f& p_pos);
	~Text();

	void resize_font(const int& p_size);
	void recolor_font(const SDL_Color& p_color);
	void change_text(const std::string& p_text);
	void change_render_mode(const int& p_render_mode);

	SDL_Renderer* renderer;
	int render_mode;

	std::string font_path;
	int size;
	TTF_Font* font;

	SDL_Color color;

	std::string text;

	Vector2f pos;
	SDL_Rect original_box;
	SDL_Rect border_box;

	SDL_Texture* texture;

private:
	void build_border_box();
};