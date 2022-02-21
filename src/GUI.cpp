#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_ttf.h>
#include <string>
#include <vector>

#include "Math.hpp"
#include "RenderWindow.hpp"
#include "utils.hpp"

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

	switch(scene)
	{
		//score board
		case 0:
			text_list = 
			{
				Text(window.get_renderer(), 5, std::string("res/fonts/Zyzol.ttf"), 100, SDL_Color{160, 160, 160, 128}, std::string("0 - 0"), Vector2f(utils::display_width() / 2.0f, utils::display_height() / 2.0f))
			};
			break;

		//main menu
		case 1:
			textbutton_list = 
			{
				TextButton(Text(window.get_renderer(), 5, std::string("res/fonts/Zyzol.ttf"), 100, SDL_Color{220, 220, 220, 255}, std::string("Start"), Vector2f(utils::display_width() / 2.0f, utils::display_height() / 2.0f)), SDL_Color{250, 250, 100, 255}, SDL_Color{200, 200, 0, 255}, 1)
			};
			break;

		//settings
		case 2:
			break;

		//pause menu
		case 3:
			break;

		//end screen
		case 4:
			text_list =
			{
				Text(window.get_renderer(), 5, std::string("res/fonts/Zyzol.ttf"), 100, SDL_Color{220, 220, 220, 255}, std::string("Player 0 Wins!"), Vector2f(utils::display_width() / 2.0f, utils::display_height() / 2.0f))
			};
			break;

		//game settings
		case 5:
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

	for(TextButton& temp_text : textbutton_list)
	{
		window.render(temp_text);
	}

}

void GUI::update()
{

}

