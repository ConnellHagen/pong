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
	GUI(RenderWindow& window);
	~GUI();

	void init_text_list(RenderWindow& window);

	void add_text(const Text& p_text);

	void render(RenderWindow& window);

	void update();

	std::vector<Text> text_list;

};
