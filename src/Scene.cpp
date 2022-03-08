#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>

#include "Scene.hpp"
#include "GUI.hpp"

Scene::Scene(RenderWindow& window, Entity* p_canvas, const SCENE_NAME& p_scene)
	:scene(p_scene), canvas(p_canvas)
{
	menu_logo_t = window.load_texture("res/images/menu-logo.png");
	menu_logo_inf = {0, 0, 1052, 288};

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
				Text(
					window.get_renderer(), 2,
					std::string("res/fonts/Zyzol.ttf"), 100,
					SDL_Color{220, 220, 220, 255},
					std::string("Start"),
					Vector2f(
						utils::display_width() / 2.0f,
						utils::display_height() / 5.0f * 3.0f
					)
				),
				SDL_Color{250, 250, 100, 255},
				SDL_Color{200, 200, 0, 255}, RESTART_GAME
			)
		};
		gui->textimage_list =
		{
			TextImage(
				Vector2f(
					utils::display_width() / 2.0f,
					utils::display_height() / 5.0f * 2.0f
				),
				Vector2f(.5, .5),
				menu_logo_t,
				menu_logo_inf,
				menu_logo_inf,
				8, SINE, 35
			)
		};
		break;
		
	case GAME:
		gui->text_list =
		{
			Text(
				window.get_renderer(), 5,
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

	case MAP_SELECTOR:
		break;

	case SETTINGS:
		break;

	case END_SCREEN:
		gui->text_list =
		{
			Text(
				window.get_renderer(), 8,
				std::string("res/fonts/Zyzol.ttf"), 100,
				SDL_Color{220, 220, 220, 255},
				std::string("Player [NULL] Wins!"),
				Vector2f(
					utils::display_width() / 2.0f,
					utils::display_height() / 2.0f
				)
			)
		};
		gui->textbutton_list =
		{
			TextButton(
				Text(
					window.get_renderer(), 2,
					std::string("res/fonts/Zyzol.ttf"), 75,
					SDL_Color{220, 220, 220, 255},
					std::string("Restart Game"),
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

	case MAP_SELECTOR:
		break;

	case SETTINGS:
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
	case GAME:
		game = new Game(window, canvas, 1, 5);
		break;

	default:
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
		std::vector<BUTTON_FUNCTION> functions = gui->update(key_pushes, mouse_coords, delta_time);

		std::vector<BUTTON_FUNCTION> functions2 = game->update(key_pushes, mouse_coords, delta_time);
		for(BUTTON_FUNCTION& func : functions2)
		{
			functions.push_back(func);
		}

		return functions;
	}
	default:
		return gui->update(key_pushes, mouse_coords, delta_time);
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

	case MAP_SELECTOR:
		break;

	case SETTINGS:
		break;

	case END_SCREEN:
		gui->render(window);
		break;
	}
}