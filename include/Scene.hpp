#pragma once

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>

#include "RenderWindow.hpp"

#include "Math.hpp"
#include "Tile.hpp"
#include "GUI.hpp"
#include "Game.hpp"

class Scene
{
public:
	Scene(RenderWindow& window, Entity* p_canvas, const SCENE_NAME& p_scene);
	~Scene();

	void init_gui(RenderWindow& window);
	void init_background(RenderWindow& window);
	void init_game(RenderWindow& window);

	void resize_display();

	std::vector<BUTTON_FUNCTION> update(const std::vector<bool>& key_pushes, const Vector2i& mouse_coords, const float& delta_time);
	void render(RenderWindow& window);

	inline Game* get_game(){return game;}
	inline GUI* get_gui(){return gui;}

private:
	SCENE_NAME scene;

	Entity* canvas;

	GUI* gui;
	Background* background;
	Game* game;

	SDL_Texture* background_t;
	SDL_Rect background_inf;
	SDL_Texture* menu_logo_t;
	SDL_Rect menu_logo_inf;
};