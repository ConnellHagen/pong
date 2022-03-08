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

//GUI is the container of `Text`, `TextButton`s and other displayed non-entities.
GUI::GUI()
{}

GUI::~GUI()
{}

void GUI::add_text(const Text& p_text)
{
	text_list.push_back(p_text);
}

void GUI::add_textbutton(const TextButton& p_textbutton)
{
	textbutton_list.push_back(p_textbutton);
}

void GUI::add_textimage(const TextImage& p_textimage)
{
	textimage_list.push_back(p_textimage);
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

	for(TextImage& temp_text : textimage_list)
	{
		window.render(temp_text);
	}
}

//accumulates all of the functions that the text buttons trigger whilst updating, and returns them in a vector
std::vector<BUTTON_FUNCTION> GUI::update(const std::vector<bool>& key_pushes, const Vector2i& mouse_coords, const float& delta_time)
{
	std::vector<BUTTON_FUNCTION> function_queue;

	for(TextButton& temp_text : textbutton_list)
	{
		std::vector<BUTTON_FUNCTION> temp = temp_text.update(key_pushes, mouse_coords);
		
		for(BUTTON_FUNCTION& func : temp)
		{
			function_queue.push_back(func);
		}
	}

	for(TextImage& temp_text : textimage_list)
	{
		std::vector<BUTTON_FUNCTION> temp = temp_text.update(delta_time);
		
		for(BUTTON_FUNCTION& func : temp)
		{
			function_queue.push_back(func);
		}
	}
	
	return function_queue;
}