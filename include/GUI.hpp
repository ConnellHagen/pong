#pragma once

#include <vector>

struct Text
{

};

struct TextButton
{
	
};

class GUI
{
public:
	GUI();

	inline int get_current_scene(){return current_scene;}
	inline void set_current_scene(const int& scene){current_scene = scene;}

	void display();

	void update();

private:
	// 0: game, 1: end screen, 2: main menu, 3: settings
	int current_scene;
	std::vector<Text> test_list;
	std::vector<TextButton> text_button_list;

};
