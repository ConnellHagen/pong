#pragma once

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <vector>

#include "Math.hpp"

class Tile
{
public:
	Tile(Vector2f const p_pos, Vector2f const p_scale, SDL_Texture* const p_texture, SDL_Rect const p_imgdata);

	inline SDL_Rect get_original_image(){return original_img;}
	
	inline Vector2f get_pos(){return pos;}

	inline SDL_Texture* get_texture(){return texture;}
	void set_texture(SDL_Texture* const &p_texture, SDL_Rect const p_imgdata);

	inline Vector2f get_scale(){return scale;}
	void set_scale(Vector2f const &p_scale);
	
private:
	SDL_Texture* texture;
	SDL_Rect original_img;
	Vector2f pos;
	Vector2f scale;
};

class Background
{
public:
	Background(Vector2f const p_scale, SDL_Texture* const p_texture, SDL_Rect const p_imgdata);

	void make_tile_list();

	inline std::vector<Tile> get_tile_list(){return tile_list;}

	inline SDL_Texture* get_texture(){return texture;}
	void set_texture(SDL_Texture* const &p_texture, SDL_Rect const p_imgdata);

	inline Vector2f get_scale(){return scale;}
	void set_scale(Vector2f const &p_scale);

private:
	SDL_Texture* texture;
	SDL_Rect original_img;
	Vector2f scale;

	std::vector<Tile> tile_list;
};