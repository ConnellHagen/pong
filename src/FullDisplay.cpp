#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <iostream>
#include <vector>
#include <cmath>

#include "utils.hpp"
#include "RenderWindow.hpp"
#include "Math.hpp"

#include "Entity.hpp"
	#include "Paddle.hpp"
	#include "Barrier.hpp"
	#include "Ball.hpp"
#include "Tile.hpp"
#include "Game.hpp"

#include "GUI.hpp"
#include "Scene.hpp"

#include "FullDisplay.hpp"

FullDisplay::FullDisplay(RenderWindow& p_window)
	:window(p_window)
{
	is_fullscreen = false;

	scene = TITLE_SCREEN;
	key_pushes = std::vector<bool>(6, false);

	SDL_Rect canvas_inf = {0, 0, utils::display::DISPLAY_WIDTH, utils::display::DISPLAY_HEIGHT};
	canvas = new Entity(Vector2f(0, 0), Vector2f(1, 1), nullptr, canvas_inf, canvas_inf, 1);

	title_screen = new Scene(window, canvas, TITLE_SCREEN);
	game = new Scene(window, canvas, GAME);
	end_screen = new Scene(window, canvas, END_SCREEN);
}

FullDisplay::~FullDisplay()
{
	delete canvas;
	delete title_screen;
	delete game;
	delete end_screen;
}

void FullDisplay::resize_display()
{
	SDL_GetWindowSize(window.get_window(), &utils::display::DISPLAY_WIDTH, &utils::display::DISPLAY_HEIGHT);

	utils::display::DISPLAY_WIDTH = static_cast<int>(utils::display::DISPLAY_HEIGHT * (static_cast<double>(utils::ORIG_DISPLAY_WIDTH) / utils::ORIG_DISPLAY_HEIGHT));
	SDL_SetWindowSize(window.get_window(), utils::display::DISPLAY_WIDTH, utils::display::DISPLAY_HEIGHT);

	SDL_SetWindowPosition(window.get_window(), SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED);

	SDL_GetWindowPosition(window.get_window(), &utils::display::DISPLAY_X, &utils::display::DISPLAY_Y);

	window.update_scale();

	title_screen->resize_display();
	game->resize_display();
	// map_select->resize_display();
	// settings->resize_display();
	end_screen->resize_display();
}

void FullDisplay::toggle_fullscreen()
{
	if(is_fullscreen)
	{
		SDL_SetWindowFullscreen(window.get_window(), 0);
		SDL_SetWindowSize(window.get_window(), utils::ORIG_DISPLAY_WIDTH, utils::ORIG_DISPLAY_HEIGHT);
		SDL_SetWindowPosition(window.get_window(), SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED);
		resize_display();
	}
	else
	{
		SDL_SetWindowFullscreen(window.get_window(), SDL_WINDOW_FULLSCREEN_DESKTOP);
		resize_display();
	}

	is_fullscreen = !is_fullscreen;
}

void FullDisplay::render()
{
	switch(scene)
	{
	case TITLE_SCREEN:
		title_screen->render(window);
		break;

	case GAME:
		game->render(window);
		break;

	case MAP_SELECTOR:
		map_select->render(window);
		break;

	case SETTINGS:
		settings->render(window);
		break;

	case END_SCREEN:
		end_screen->render(window);
		break;
	}
}

void FullDisplay::update(const float& delta_time)
{
	std::vector<BUTTON_FUNCTION> function_queue;

	switch(scene)
	{
	case TITLE_SCREEN:
		function_queue = title_screen->update(key_pushes, mouse_coords, delta_time);
		break;
		
	case GAME:
		function_queue = game->update(key_pushes, mouse_coords, delta_time);
		break;

	case MAP_SELECTOR:
		function_queue = map_select->update(key_pushes, mouse_coords, delta_time);
		break;

	case SETTINGS:
		function_queue = settings->update(key_pushes, mouse_coords, delta_time);
		break;
		
	case END_SCREEN:
		function_queue = end_screen->update(key_pushes, mouse_coords, delta_time);
		break;
	}

	for(BUTTON_FUNCTION& func : function_queue)
	{
		execute_function(func);
	}
}

void FullDisplay::update_keys(const SDL_Event* event)
{
	switch(event->type)
	{
	case SDL_KEYDOWN:
		switch (event->key.keysym.sym)
		{
	    case SDLK_w:
	    	key_pushes[0] = true;
	    	break;

	    case SDLK_s:
	    	key_pushes[1] = true;
	    	break;

	    case SDLK_UP:
		    key_pushes[2] = true;
		    break;

	    case SDLK_DOWN:
		    key_pushes[3] = true;
		    break;
		case SDLK_F11:
			toggle_fullscreen();
			break;
		}
		break;

	case SDL_KEYUP:
		switch (event->key.keysym.sym)
		{
		case SDLK_w:
	    	key_pushes[0] = false;
	    	break;

	    case SDLK_s:
	    	key_pushes[1] = false;
	    	break;

	    case SDLK_UP:
		    key_pushes[2] = false;
		    break;
		    
	    case SDLK_DOWN:
		    key_pushes[3] = false;
		    break;
		}
		break;

	case SDL_MOUSEBUTTONDOWN:
	    switch (event->button.button)
	    {
        case SDL_BUTTON_LEFT:
        	key_pushes[4] = true;
            break;

        case SDL_BUTTON_RIGHT:
        	key_pushes[5] = true;
        	break;
	    }
	    break;

	case SDL_MOUSEBUTTONUP:
	    switch (event->button.button)
	    {
        case SDL_BUTTON_LEFT:
        	key_pushes[4] = false;
            break;
        case SDL_BUTTON_RIGHT:
        	key_pushes[5] = false;
        	break;
	    }
	    break;

	case SDL_MOUSEMOTION:
		mouse_coords = shift_coords(Vector2i(event->motion.x, event->motion.y));
		break;

	default: break;
	}
}


Vector2i FullDisplay::shift_coords(const Vector2i& coords)
{
	double ratio_x = utils::ORIG_DISPLAY_WIDTH / static_cast<double>(utils::display::DISPLAY_WIDTH);
	double ratio_y = utils::ORIG_DISPLAY_HEIGHT / static_cast<double>(utils::display::DISPLAY_HEIGHT);

	int new_x = static_cast<int>(coords.x * ratio_x);
	int new_y = static_cast<int>(coords.y * ratio_y);

	Vector2i new_coords = Vector2i(new_x, new_y);

	return new_coords;
}


void FullDisplay::execute_function(const BUTTON_FUNCTION& func)
{
	switch(func)
	{
	case RESTART_GAME:
		game->get_game()->restart_game();
		game->get_gui()->text_list[0].change_text(std::string("0 - 0"));
		scene = GAME;
		break;

	case OPEN_SETTINGS:
		menu_stack.push(scene);
		scene = SETTINGS;
		break;

	case OPEN_MAP_SELECT:
		menu_stack.push(scene);
		scene = MAP_SELECTOR;
		break;

	case EXIT_MENU:
		scene = menu_stack.top();
		menu_stack.pop();
		break;

	case PLAYER_1_GOAL:
	case PLAYER_2_GOAL:
	{
		Game* temp_game = game->get_game();

		std::string new_score(std::to_string(temp_game->get_score().x) + " - " + std::to_string(temp_game->get_score().y));
		game->get_gui()->text_list[0].change_text(new_score);
		break;
	}

	case PLAYER_1_WIN:
		end_screen->get_gui()->text_list[0].change_text(std::string("Player 1 Wins!"));
		scene = END_SCREEN;		
		break;

	case PLAYER_2_WIN:
		end_screen->get_gui()->text_list[0].change_text(std::string("Player 2 Wins!"));
		scene = END_SCREEN;
		break;

	default: break;
	}
}