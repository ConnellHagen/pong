#pragma once
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>

#include "Math.hpp"
#include "Entity.hpp"
#include "Tile.hpp"
#include "Ball.hpp"
#include "Paddle.hpp"

class RenderWindow
{
public:
	RenderWindow(const char* p_title, int p_w, int p_h, Vector2f p_scalar);
	int get_refresh_rate();
	SDL_Texture* load_texture(const char* p_filePath);
	void clean_up();
	void clear();

	void render(Entity p_entity);
	void render(Tile p_tile);
	void render(Background p_background);

	void display();

	inline void set_universal_scalar(Vector2f const &scale){universal_scalar = scale;}
	inline Vector2f get_universal_scalar(){return universal_scalar;}

private:
	SDL_Window* window;
	SDL_Renderer* renderer;

	Vector2f universal_scalar;

};