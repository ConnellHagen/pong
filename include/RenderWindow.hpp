#pragma once

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_ttf.h>

#include "Math.hpp"
#include "Tile.hpp"
#include "Entity.hpp"
	#include "Ball.hpp"
	#include "Paddle.hpp"
#include "GUI.hpp"

class RenderWindow
{
public:
	RenderWindow(const char* p_title, int p_w, int p_h, Vector2f p_scalar);
	int get_refresh_rate();
	SDL_Texture* load_texture(const char* p_filePath);

	void clear();

	void render(Entity p_entity);
	void render(Background p_background);
	void render(Tile p_tile);
	void render(GUI p_gui);
	void render(Text p_text);
	void render(TextButton p_text);

	void display();

	inline void set_universal_scalar(Vector2f const &scale){universal_scalar = scale;}
	inline Vector2f get_universal_scalar(){return universal_scalar;}
	
	void clean_up();

private:
	SDL_Window* window;
	SDL_Renderer* renderer;

	Vector2f universal_scalar;

};