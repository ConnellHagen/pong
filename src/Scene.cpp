#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>

#include "Scene.hpp"
#include "GUI.hpp"

Scene::Scene(RenderWindow& window, Entity* p_canvas, const SCENE_NAME& p_scene)
	:scene(p_scene), canvas(p_canvas)
{
	init_gui(window);
	init_background(window);
	init_game(window);
}

Scene::~Scene()
{
	SDL_DestroyTexture(background_t);
	delete gui;
	delete background;
	delete game;
}

void Scene::init_gui(RenderWindow& window)
{
	gui = new GUI();

	switch(scene)
	{
	case TITLE_SCREEN:
		gui->textbutton_list = 
		{
			TextButton(
				Text(window.get_renderer(), 5,
					std::string("res/fonts/Zyzol.ttf"), 100,
					SDL_Color{220, 220, 220, 255},
					std::string("Start"),
					Vector2f(
						utils::display_width() / 2.0f,
						utils::display_height() / 2.0f
					)
				),
				SDL_Color{250, 250, 100, 255},
				SDL_Color{200, 200, 0, 255}, RESTART_GAME
			)
		};
		break;
		
	case GAME:
		gui->text_list =
		{
			Text(window.get_renderer(), 5,
				std::string("res/fonts/Zyzol.ttf"), 100,
				SDL_Color{160, 160, 160, 128},
				std::string("0 - 0"),
				Vector2f(
					utils::display_width() / 2.0f,
				 	utils::display_height() / 2.0f
				)
			)
		};
		break;

	case END_SCREEN:
		gui->text_list =
		{
			Text(window.get_renderer(), 5,
				std::string("res/fonts/Zyzol.ttf"), 100,
				SDL_Color{220, 220, 220, 255},
				std::string("Player 0 Wins!"),
				Vector2f(
					utils::display_width() / 2.0f,
					utils::display_height() / 2.0f
				)
			)
		};
		break;
	}
}

void Scene::init_background(RenderWindow& window)
{
	switch(scene)
	{
	case TITLE_SCREEN:
		background_t = window.load_texture("res/images/background.png");
		background_inf = {0, 0, 32, 32};
		background = new Background(Vector2f(4, 4), background_t, background_inf);
		break;

	case GAME:
		background_t = window.load_texture("res/images/background.png");
		background_inf = {0, 0, 32, 32};
		background = new Background(Vector2f(4, 4), background_t, background_inf);
		break;

	case END_SCREEN:
		background_t = window.load_texture("res/images/background.png");
		background_inf = {0, 0, 32, 32};
		background = new Background(Vector2f(4, 4), background_t, background_inf);
		break;
	}
}

void Scene::init_game(RenderWindow& window)
{
	switch(scene)
	{
	case TITLE_SCREEN:
		game = nullptr;
		break;

	case GAME:
		game = new Game(window, canvas, 1, 5);
		break;

	case END_SCREEN:
		game = nullptr;
		break;
	}
}

std::vector<BUTTON_FUNCTION> Scene::update(const std::vector<bool>& key_pushes, const Vector2i& mouse_coords, const float& delta_time)
{
	switch(scene)
	{
	case GAME:
	{
		std::vector<BUTTON_FUNCTION> functions = gui->update(key_pushes, mouse_coords);

		std::vector<BUTTON_FUNCTION> functions2 = game->update(key_pushes, mouse_coords, delta_time);
		for(BUTTON_FUNCTION& func : functions2)
		{
			functions.push_back(func);
		}

		return functions;
	}
	default:
		return gui->update(key_pushes, mouse_coords);
	}
}

void Scene::render(RenderWindow& window)
{
	window.render(*background);

	switch(scene)
	{
	case TITLE_SCREEN:
		gui->render(window);
		break;

	case GAME:
		gui->render(window);
		game->render(window);
		break;

	case END_SCREEN:
		gui->render(window);
		break;
	}
}