#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_ttf.h>
#include <string>
#include <vector>

#include "Math.hpp"
#include "RenderWindow.hpp"
#include "Text.hpp"

#include "GUI.hpp"

GUI::GUI(RenderWindow& window)
{
	init_text_list(window);
}
GUI::~GUI()
{

}

void GUI::init_text_list(RenderWindow& window)
{
	text_list.clear();

	text_list.push_back(Text(window.get_renderer(), std::string("res/fonts/DenseLetters.ttf"), 70, SDL_Color{255, 255, 128}, std::string("Hello World"), Vector2f(100, 200)));
}

void GUI::add_text(const Text& p_text)
{
	text_list.push_back(p_text);
}

void GUI::render(RenderWindow& window)
{
	for(Text& temp_text : text_list)
	{
		window.render(temp_text);
	}

}

void GUI::update()
{

}

