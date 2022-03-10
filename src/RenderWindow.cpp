#include <iostream>
#include <string>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_ttf.h>

#include "Math.hpp"
#include "Tile.hpp"
#include "Entity.hpp"
	#include "Ball.hpp"
	#include "Paddle.hpp"
#include "Text.hpp"

#include "RenderWindow.hpp"

RenderWindow::RenderWindow(const char* p_title, const int& p_w, const int& p_h, const Vector2f& p_scale)
	:window(NULL), renderer(NULL), scale(p_scale)
{
	window = SDL_CreateWindow(p_title, SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, p_w, p_h, SDL_WINDOW_SHOWN | SDL_WINDOW_RESIZABLE);
	
	if(window == NULL)
	{
		std::cout << "Window failed to init. Error: " << SDL_GetError() << "\n";
	}

	renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
}

SDL_Texture* RenderWindow::load_texture(const char* p_filePath)
{
	SDL_Texture* texture = NULL;
	texture = IMG_LoadTexture(renderer, p_filePath);

	if(texture == NULL)
		std::cout << "Failed to load texture. Error: " << SDL_GetError() << "\n";

	return texture;

}
 
int RenderWindow::get_refresh_rate()
{
	int displayIndex = SDL_GetWindowDisplayIndex(window);
	SDL_DisplayMode mode;
	SDL_GetDisplayMode(displayIndex, 0, &mode);
	
	return mode.refresh_rate;
}

void RenderWindow::clear()
{
	SDL_RenderClear(renderer);
}
	
/*
render_mode determines what part of the rectangle (x,y) is (corner, edge or center)
|1-2-3|
|4-5-6|
|7-8-9|
*/
void RenderWindow::render(Entity p_entity)
{
	SDL_Rect src;
	src.x = p_entity.get_sprite_frame().x;
	src.y = p_entity.get_sprite_frame().y;
	src.w = p_entity.get_sprite_frame().w;
	src.h = p_entity.get_sprite_frame().h;

	SDL_Rect dst = p_entity.get_border_box();
	dst.x *= scale.x;
	dst.y *= scale.y;
	dst.w *= scale.x;
	dst.h *= scale.y;

	SDL_RenderCopyEx(renderer, p_entity.get_texture(), &src, &dst, static_cast<double>(p_entity.get_angle()), NULL, SDL_FLIP_NONE);
}

void RenderWindow::render(Background& p_background)
{
	std::vector<Tile> render_list = p_background.get_tile_list();
	for(Tile background_tile : render_list)
	{
		render(background_tile);
	}
}

void RenderWindow::render(Tile& p_tile)
{
	SDL_Rect src;
	src.x = p_tile.get_original_image().x;
	src.y = p_tile.get_original_image().y;
	src.w = p_tile.get_original_image().w;
	src.h = p_tile.get_original_image().h;

	SDL_Rect dst;
	dst.x = p_tile.get_pos().x * scale.x;
	dst.y = p_tile.get_pos().y * scale.y;
	dst.w = p_tile.get_original_image().w * p_tile.get_scale().x * scale.x;
	dst.h = p_tile.get_original_image().h * p_tile.get_scale().y * scale.y;

	SDL_RenderCopy(renderer, p_tile.get_texture(), &src, &dst);
}

void RenderWindow::render(Text& p_text)
{
	SDL_Rect src = p_text.border_box;
	src.x = 0;
	src.y = 0;

	SDL_Rect dst = p_text.border_box;
	dst.x *= scale.x;
	dst.y *= scale.y;
	dst.w *= scale.x;
	dst.h *= scale.y;

	SDL_RenderCopyEx(renderer, p_text.texture, &src, &dst, 0, NULL, SDL_FLIP_NONE);
}

void RenderWindow::render(TextButton& p_text)
{
	render(p_text.text);
}

void RenderWindow::render(TextImage& p_text)
{
	SDL_Rect src;
	src.x = p_text.current_sprite_frame.x;
	src.y = p_text.current_sprite_frame.y;
	src.w = p_text.current_sprite_frame.w;
	src.h = p_text.current_sprite_frame.h;

	SDL_Rect dst = p_text.border_box;
	dst.x *= scale.x;
	dst.y *= scale.y;
	dst.w *= scale.x;
	dst.h *= scale.y;

	SDL_RenderCopyEx(renderer, p_text.texture, &src, &dst, static_cast<double>(p_text.angle), NULL, SDL_FLIP_NONE);
}
	
void RenderWindow::display()
{
	SDL_RenderPresent(renderer);
}

void RenderWindow::update_scale()
{
	scale.x = utils::display::DISPLAY_WIDTH / static_cast<float>(utils::ORIG_DISPLAY_WIDTH);
	scale.y = utils::display::DISPLAY_HEIGHT / static_cast<float>(utils::ORIG_DISPLAY_HEIGHT);
}

void RenderWindow::clean_up()
{
	SDL_DestroyWindow(window);
}
