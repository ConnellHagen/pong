#pragma once

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include "GUI.hpp"
#include "Tile.hpp"
#include "Math.hpp"

class Scene
{
public:
	Scene(RenderWindow& window, const GUI& p_scene_gui);

	void init_background();

	void update(const std::vector<bool>& key_pushes, const Vector2i& mouse_coords, int* game_scene);
	void render(RenderWindow& window);

	SDL_Texture* background_t;

	SDL_Rect background_inf;

private:
	GUI scene_gui;

	Background background;

};