#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <iostream>
#include <vector>
#include <cmath>
#include <stack>

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

class FullDisplay
{
public:
	FullDisplay(RenderWindow& p_window);
	~FullDisplay();

	void render();
	void update(const float& delta_time);

	void update_keys(const SDL_Event* event);

private:
	void execute_function(const BUTTON_FUNCTION& func);

	std::stack<SCENE_NAME> menu_stack;

	//W, S, UP, DOWN, Left Click, Right Click
	std::vector<bool> key_pushes;
	Vector2i mouse_coords;

	SCENE_NAME scene;

	RenderWindow window;

	//All scenes etc.
	Entity* canvas;

	Scene* title_screen;
	Scene* game;
	Scene* map_select;
	Scene* settings;
	Scene* end_screen;
};