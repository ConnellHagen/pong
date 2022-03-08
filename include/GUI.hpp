#pragma once

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_ttf.h>
#include <vector>
#include <string>

#include "Math.hpp"
#include "RenderWindow.hpp"
#include "Text.hpp"

enum SCENE_NAME{TITLE_SCREEN, GAME, MAP_SELECTOR, SETTINGS, END_SCREEN};

struct GUI
{
	GUI();
	~GUI();

	void add_text(const Text& p_text);
	void add_textbutton(const TextButton& p_textbutton);
	void add_textimage(const TextImage& p_textimage);

	void render(RenderWindow& window);

	std::vector<BUTTON_FUNCTION> update(const std::vector<bool>& key_pushes, const Vector2i& mouse_coords, const float& delta_time);

	std::vector<Text> text_list;
	std::vector<TextButton> textbutton_list;
	std::vector<TextImage> textimage_list;

};
