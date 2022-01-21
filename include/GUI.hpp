#pragma once

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_ttf.h>
#include <vector>

#include "Math.hpp"

struct Text
{
	Text(char* p_text, const int& p_size, TTF_Font* p_font, const SDL_Color& p_color, const Vector2f& p_position, const int& p_render_mode);
	~Text();

	char* text;
	int size;
	TTF_Font* font;
	SDL_Color color;
	Vector2f position;
	int render_mode;
};

struct TextButton
{
	TextButton(char* p_text, const int& p_size, TTF_Font* p_font, const SDL_Color& p_reg_color, const SDL_Color& p_press_color, const Vector2f& p_position, const int& p_render_mode, const int& p_id);
	~TextButton();

	SDL_Color get_active_color();

	char* text;
	int size;
	TTF_Font* font;
	SDL_Color reg_color;
	SDL_Color press_color;
	Vector2f position;
	int render_mode;
	int id;
	bool is_pressed;
	
};

class GUI
{
public:
	GUI();

	inline int get_current_scene(){return current_scene;}
	inline void set_current_scene(const int& scene){current_scene = scene;}

	inline std::vector<Text> get_text_list(const int& scene){return text_list[scene];}
	inline std::vector<TextButton> get_text_button_list(const int& scene){return text_button_list[scene];}

	void add_text(const int& scene, const Text& p_text);
	void add_text_button(const int& scene, const TextButton& p_text);

	void display();

	void update();

private:
	// 0: game, 1: end screen, 2: main menu, 3: settings
	int current_scene;
	std::vector<std::vector<Text>> text_list;
	std::vector<std::vector<TextButton>> text_button_list;

};
