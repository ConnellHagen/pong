#pragma once

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>

#include "Math.hpp"

class Entity
{
public:
	Entity(Vector2f const &p_pos, Vector2f const &p_scale, SDL_Texture* const &p_texture, SDL_Rect p_imgdata)
	:original_img(p_imgdata), texture(p_texture), pos(p_pos), scale(p_scale)
	{}

	inline Vector2f& get_pos(){return pos;}
	inline void set_pos(Vector2f const &p_pos){pos = p_pos;}

	inline float get_angle(){return angle;}
	inline void set_angle(float const &p_angle){angle = p_angle;}

	inline Vector2f& get_scale(){return scale;}
	inline void set_scale(float const &p_w, float const &p_h){scale = Vector2f(p_w, p_h);}

	inline SDL_Texture* get_texture(){return texture;}

	// inline SDL_Rect get_current_image(){return current_img;}
	// inline void set_current_image(SDL_Rect const &p_img){current_img = p_img;}

	inline SDL_Rect get_original_image(){return original_img;}
	inline void set_original_image(SDL_Rect const &p_imgdata){original_img = p_imgdata;}

private:
	float angle = 0;
	SDL_Rect original_img;
	SDL_Texture* texture;
	Vector2f pos;
	Vector2f scale;

};