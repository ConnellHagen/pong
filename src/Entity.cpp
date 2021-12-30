#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>

#include "Entity.hpp"

Entity::Entity(Vector2f p_pos, Vector2f p_scale, SDL_Texture* p_texture)
	:texture(p_texture), pos(p_pos), scale(p_scale)
{}