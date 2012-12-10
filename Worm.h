#ifndef WORM_H
#define WORM_H

#include "Position_class.h"
#include "SDL_stdinc.h"
#include <iostream>

class Worm {

private:
	enum{
		default_thickness = 5,
		default_powerup_timer = 0
	};

	const double default_speed = 0.1;
	int distance_to_hole;
	Uint32 colour;
	int left_control;
	int right_control;
	Position_class* position;
	double direction;
	int score = 0;
public:
	double speed = default_speed;
	std::string team;
	double thickness = default_thickness;
	int powerup_timer = default_powerup_timer;
	bool powerup_through_wall = false;
	bool powerup_ghost = false;
	bool powerup_sharp_turn = false;
	bool powerup_mirror = false;

	Worm(Uint32 colour, int left_control, int right_control, int window_height);

private:
	int random_distance_to_hole();

public:
	~Worm();

	void set_powerup_timer();

	void change_direction(double);

	int get_direction();

	void kill_worm();

	Position_class* get_position();

	Uint32 get_colour();

	int get_left_control();

	void add_score();

	int get_score();

	int get_right_control();

	int get_distance_to_hole();

	void move();

	void set_position(double, double);

	void reset_worm();

	void random_position(int);
};

#endif /* WORM_H */
