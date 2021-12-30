#pragma once
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>

#include "Math.hpp"

class Entity
{
public:
	Entity(Vector2f p_pos, Vector2f p_scale, SDL_Texture* p_texture);

	// inline Vector2f& getPos(){return pos;}
	// inline void setPos(float const &p_x, float const &p_y){pos = Vector2f(p_x, p_y);}

	inline float getAngle(){return angle;}
	inline void setAngle(float const &p_angle){angle = p_angle;}

	inline Vector2f& getScale(){return scale;}
	inline void setScale(float const &p_w, float const &p_h){scale = Vector2f(p_w, p_h);}

	inline SDL_Texture* getTexture(){return texture;}

	inline SDL_Rect getCurrentFrame(){return currentFrame;}
	inline void setCurrentFrame(SDL_Rect const &frame){currentFrame = frame;}

	inline void setOriginalImage(SDL_Rect const &imgdata){originalImg = imgdata;}
	inline SDL_Rect getOriginalImage(){return originalImg;}

private:
	float angle = 0;
	SDL_Rect currentFrame;
	SDL_Rect originalImg;
	SDL_Texture* texture;
	Vector2f pos;
	Vector2f scale;

};