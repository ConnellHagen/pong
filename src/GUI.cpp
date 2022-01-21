#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_ttf.h>
#include <vector>

#include "GUI.hpp"

void GUI::add_text(const int& scene, const Text& p_text)
{
	text_list[scene].push_back(p_text);
}

void GUI::add_text_button(const int& scene, const TextButton& p_text)
{
	text_button_list[scene].push_back(p_text);
}

Text::Text(char* p_text, const int& p_size, TTF_Font* p_font, const SDL_Color& p_color, const Vector2f& p_position, const int& p_render_mode)
:text(p_text), size(p_size), font(p_font), color(p_color), position(p_position), render_mode(p_render_mode)
{}

Text::~Text()
{
	//TTF_CloseFont(font);
}

TextButton::TextButton(char* p_text, const int& p_size, TTF_Font* p_font, const SDL_Color& p_reg_color, const SDL_Color& p_press_color, const Vector2f& p_position, const int& p_render_mode, const int& p_id)
:text(p_text), size(p_size), font(p_font), reg_color(p_reg_color), press_color(p_press_color), position(p_position), render_mode(p_render_mode), id(p_id), is_pressed(false)
{}

TextButton::~TextButton()
{
	//TTF_CloseFont(font);
}

SDL_Color TextButton::get_active_color()
{
	return (is_pressed ? reg_color : press_color);
}