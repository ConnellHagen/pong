#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_ttf.h>
#include <string>
#include <vector>

#include "Math.hpp"
#include "RenderWindow.hpp"
#include "Text.hpp"

#include "GUI.hpp"

//GUI is the container of `Text`, `Button`s and other displayed non-entities.
GUI::GUI(RenderWindow& window, const int& scene)
{
	init_text_list(window, scene);
}

GUI::~GUI()
{

}

void GUI::init_text_list(RenderWindow& window, const int& scene)
{
	text_list.clear();

	std::cout << scene << "\n";

	switch(scene)
	{
		case 0:
		std::cout << "here\n";
			text_list = 
			{
				Text(window.get_renderer(), 5, std::string("res/fonts/DenseLetters.ttf"), 70, SDL_Color{255, 255, 128}, std::string("Hello World"), Vector2f(100, 200))
			};
			break;
	}

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

