#pragma once

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_ttf.h>

#include "Math.hpp"
#include "Tile.hpp"
#include "Entity.hpp"
	#include "Ball.hpp"
	#include "Paddle.hpp"
#include "Text.hpp"

class RenderWindow
{
public:
	RenderWindow(const char* p_title, const int& p_w, const int& p_h, const Vector2f& p_scale);
	int get_refresh_rate();

	SDL_Texture* load_texture(const char* p_filePath);

	void clear();

	void render(Entity p_entity);
	void render(Background& p_background);
	void render(Tile& p_tile);
	void render(Text& p_text);
	void render(TextButton& p_text);
	void render(TextImage& p_text);

	void display();

	void update_scale();

	inline SDL_Renderer*& get_renderer(){return renderer;}
	
	void clean_up();

private:
	SDL_Window* window;
	SDL_Renderer* renderer;

	Vector2f scale;

};