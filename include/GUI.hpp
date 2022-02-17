#pragma once

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_ttf.h>
#include <vector>
#include <string>

#include "Math.hpp"
#include "RenderWindow.hpp"
#include "Text.hpp"

struct GUI
{
	GUI(RenderWindow& window, const int& scene);
	~GUI();

	void init_text_list(RenderWindow& window, const int& scene);
	void init_textbutton_list();

	void add_text(const Text& p_text);
	void add_textbutton(const TextButton& p_textbutton);

	// inline std::vector<Text> get_text_list(){return text_list;}
	// inline std::vector<TextButton> get_textbutton_list(){return textbutton_list;}

	void render(RenderWindow& window);

	void update();

	std::vector<Text> text_list;
	std::vector<TextButton> textbutton_list;

};
