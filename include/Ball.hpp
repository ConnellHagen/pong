#pragma once

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <vector>

#include "Math.hpp"

#include "Entity.hpp"
	#include "Barrier.hpp"
	#include "Paddle.hpp"
	#include "Ball.hpp"

class Paddle;

enum GOAL_SCORE{NO_SCORE, P1_SCORE, P2_SCORE};

class Ball : public Entity
{
public:
	Ball(const Vector2f& p_pos, const Vector2f& p_scale, SDL_Texture* p_texture, const SDL_Rect& p_sheet, const SDL_Rect& p_current, const int& p_render_mode);

	inline Vector2f get_velocity(){return velocity;}
	inline void set_velocity(const Vector2f& p_velocity){velocity = p_velocity;}
	void bounce_y_velocity(float& percent_off);
	void scale_velocity(const Vector2f& p_scale);
	void set_velocity_dir(const Vector2f& p_direction);

	inline float get_rotation_velocity(){return rotation_velocity;}
	void random_rotation_velocity();
	void set_rotation_direction(const int& direction);
	int get_rotation_direction();
	inline void reverse_rotation_direction(){rotation_velocity *= -1;}

	Vector2f next_pos();
	
	GOAL_SCORE update(Entity* canvas, std::vector<Paddle> paddle_list, std::vector<Barrier> barrier_list, const float& delta_time);

private:
	Vector2f velocity;
	float rotation_velocity;
	int net_velocity;

	void update_x_velocity();
};