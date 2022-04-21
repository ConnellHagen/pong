#include <cmath>

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <vector>

#include "Math.hpp"
#include "utils.hpp"

#include "Tile.hpp"


Tile::Tile(Vector2f const p_pos, Vector2f const p_scale, SDL_Texture* const p_texture, SDL_Rect const p_imgdata)
	:texture(p_texture), original_img(p_imgdata), pos(p_pos), scale(p_scale)
{}

void Tile::set_texture(SDL_Texture* const &p_texture, SDL_Rect const p_imgdata)
{
	texture = p_texture;
	original_img = p_imgdata;
}

void Tile::set_scale(Vector2f const &p_scale)
{
	scale = p_scale;
}


Background::Background(Vector2f const p_scale, SDL_Texture* const p_texture, SDL_Rect const p_imgdata)
	:texture(p_texture), original_img(p_imgdata),  scale(p_scale)
{
	make_tile_list();
}

Background::Background()
{}

void Background::make_tile_list()
{
	tile_list.clear();

	const int tile_width = std::floor(scale.x * original_img.w);
	const int tile_height = std::floor(scale.y * original_img.h);

	const int horizontal_tiles = std::ceil(utils::ORIG_DISPLAY_WIDTH / static_cast<float>(tile_width));
	const int vertical_tiles = std::ceil(utils::ORIG_DISPLAY_HEIGHT / static_cast<float>(tile_height));

	for(int i = 0; i < horizontal_tiles; i++)
	{
		for(int j = 0; j < vertical_tiles; j++)
		{
			tile_list.push_back(Tile(
				Vector2f(i * tile_width, j * tile_height),
				scale,
				texture,
				original_img
			));

		}
	}
}

void Background::set_texture(SDL_Texture* const &p_texture, SDL_Rect const p_imgdata)
{
	texture = p_texture;
	original_img = p_imgdata;
	make_tile_list();
}

void Background::set_scale(Vector2f const &p_scale)
{
	scale = p_scale;
	make_tile_list();
}