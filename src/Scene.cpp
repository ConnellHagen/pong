#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>

#include "Scene.hpp"
#include "GUI.hpp"


Scene::Scene(RenderWindow& window, const GUI& p_scene_gui)
	:scene_gui(p_scene_gui)
{
	background_t = window.load_texture("res/images/background.png");

	background_inf = {0, 0, 32, 32};

	init_background();
}

void Scene::init_background()
{
	background = Background(Vector2f(4, 4), background_t, background_inf);
}


void Scene::update(const std::vector<bool>& key_pushes, const Vector2i& mouse_coords, int* game_scene)
{
	for(TextButton& button : scene_gui.textbutton_list)
	{
		button.update(key_pushes, mouse_coords, game_scene);
	}
	
}

void Scene::render(RenderWindow& window)
{
	window.render(background);

	scene_gui.render(window);
}

EndScreen::EndScreen(RenderWindow& window, GUI p_scene_gui)
	:Scene(window, p_scene_gui)
{

}

void EndScreen::set_winner(const int& p_winner)
{
	winner = p_winner;
	Scene::get_gui().text_list[0].change_text("Player " + std::to_string(winner) + " Wins!");

}
