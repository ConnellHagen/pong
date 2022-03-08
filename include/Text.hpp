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

enum BUTTON_FUNCTION{RESTART_GAME, OPEN_SETTINGS, OPEN_MAP_SELECT, EXIT_MENU, PLAYER_1_GOAL, PLAYER_2_GOAL, PLAYER_1_WIN, PLAYER_2_WIN};

struct TextButton
{
public:
	TextButton(const Text& p_text, const SDL_Color& p_hover_color, const BUTTON_FUNCTION& p_press_function);
	TextButton(const Text& p_text, const SDL_Color& p_hover_color, const SDL_Color& p_press_color, const BUTTON_FUNCTION& p_press_function);
	~TextButton();

	std::vector<BUTTON_FUNCTION> update(const std::vector<bool>& key_pushes, const Vector2i& mouse_coords);

	inline SDL_Rect get_border_box(){return text.border_box;}

	Text text;

	enum color_state{UNPRESSED = 0, PRESSED = 1, HOVER = 2};
	color_state current_state;

	SDL_Color default_color;
	SDL_Color hover_color;
	SDL_Color press_color;

	BUTTON_FUNCTION press_function;

private:
};


enum MOVEMENT_TYPE{NONE, SINE};

struct TextImage
{
	TextImage(const Vector2f& p_pos, const Vector2f& p_scale, SDL_Texture* p_texture, const SDL_Rect& p_sheet, const SDL_Rect& p_current, const int& p_render_mode, const MOVEMENT_TYPE& p_movement, const int& p_magnitude);

	std::vector<BUTTON_FUNCTION> update(const float& delta_time);

	// inline float get_angle(){return angle;}
	// inline void set_angle(const float& p_angle){angle = p_angle;}
	// inline void rotate_deg(const float& p_rotation){angle += p_rotation;}

	// inline Vector2f& get_scale(){return scale;}
	// void set_scale(const float& p_w, const float& p_h);

	inline SDL_Texture* get_texture(){return texture;}

	// inline SDL_Rect get_sprite_frame(){return current_sprite_frame;}
	// inline void set_sprite_frame(const SDL_Rect& p_imgdata){current_sprite_frame = p_imgdata;}

	void next_sprite_frame();

	//void move(const Vector2f& increment);

	float angle = 0;
	SDL_Rect sprite_sheet;
	SDL_Rect current_sprite_frame;
	SDL_Texture* texture;
	Vector2f original_pos;
	Vector2f pos;
	Vector2f scale;
	MOVEMENT_TYPE movement;
	int magnitude;

	int render_mode;
	SDL_Rect border_box;

	float accumulator;

private:
	void build_border_box();
	void move();
};