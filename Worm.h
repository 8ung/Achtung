#ifndef WORM_H
#define WORM_H

#include "Position_class.h"
#include "SDL_stdinc.h"
#include <iostream>

class Worm {

private:
	const double default_powerup_timer = 0;
	const double default_thickness = 5;
	const double default_speed = 0.1;
	int distance_to_hole;
	Uint32 colour;
	int left_control;
	int right_control;
	double direction;
	int score = 0;

	int random_distance_to_hole();

public:
	Position_class* position;
	double speed = default_speed;
	std::string team;
	double thickness = default_thickness;
	int powerup_timer = default_powerup_timer;
	bool powerup_through_wall = false;
	bool powerup_mirror = false;

	Worm(const Uint32 colour, const int left_control,
			const int right_control, const int window_height);

	~Worm();

	const Uint32 get_colour();

	const int get_left_control();

	const int get_right_control();

	void change_direction(const double);

	const int get_direction();

	void set_position(const double, const double);

	void move();

	void random_position(const int);

	void kill_worm();

	void add_score();

	const int get_score();

	const int get_distance_to_hole();

	void set_powerup_timer();

	void reset_worm();
};

#endif /* WORM_H */
