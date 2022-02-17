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

struct TextButton
{
public:
	TextButton(const Text& p_text, const SDL_Color& p_hover_color, const int& p_ID);
	TextButton(const Text& p_text, const SDL_Color& p_hover_color, const SDL_Color& p_press_color, const int& p_ID);
	~TextButton();

	void update(const std::vector<bool>& key_pushes, const Vector2i& mouse_coords, int* game_scene);

	void set_color(const SDL_Color& p_color);

	inline SDL_Rect get_border_box(){return text.border_box;}

	Text text;

	enum color_state{UNPRESSED = 0, PRESSED = 1, HOVER = 2};
	color_state current_state;

	SDL_Color default_color;
	SDL_Color hover_color;
	SDL_Color press_color;

	int ID;

private:
	void button_pushed(int* game_scene);
	void button_released(int* game_scene);

};