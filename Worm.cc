#include "Worm.h"
#include <math.h>
#include <time.h>

Worm::Worm(Uint32 new_colour, int new_left_control, int new_right_control, int window_height) {
	distance_to_hole = random_distance_to_hole();
	colour = new_colour;
	left_control = new_left_control;
	right_control = new_right_control;
	position = new Position_class();
	position->random_position(window_height);
	//srand(time(NULL));
	direction = rand() % 360;
	if(colour > 70000)
	{
		team = "hot";
	}
	else
	{
		team = "cold";
	}
}

int Worm::random_distance_to_hole()
{
	int distance = rand() % 5000;
	return distance;
}

Worm::~Worm()
{
	delete[] position;
}

int Worm::get_distance_to_hole()
{
	return distance_to_hole;
}

void Worm::random_position(int window_height)
{
	position->random_position(window_height);
}

void Worm::change_direction(double degrees)
{
	direction = direction - degrees;
	if(direction >= 360)
	{
		direction = direction - 360;
	}
	else if(direction <= 0)
	{
		direction = direction + 360;
	}
}

int Worm::get_direction()
{
	return direction;
}

void Worm::kill_worm()
{
	speed = 0;
}

Position_class* Worm::get_position()
{
	return position;
}
void Worm::set_position(double x, double y)
{
	position->x_koord = x;
	position->y_koord = y;
}

Uint32 Worm::get_colour()
{
	return colour;
}

int Worm::get_left_control()
{
	return left_control;
}

int Worm::get_right_control()
{
	return right_control;
}

void Worm::add_score()
{
	score++;
}

int Worm::get_score()
{
	return score;
}
void Worm::move()
{
	if(distance_to_hole == -350)
	{
		distance_to_hole = random_distance_to_hole();
	}
	position->x_koord = position->x_koord + speed*cos(direction*3.141592/180);
	position->y_koord = position->y_koord + speed*sin(direction*3.141592/180);
	distance_to_hole--;
	powerup_timer--;
	if(powerup_timer == 0)
	{
		reset_worm();
	}
}

void Worm::reset_worm()
{
	speed = default_speed;
	thickness = default_thickness;
	powerup_timer = default_powerup_timer;
	powerup_through_wall = false;
	powerup_ghost = false;
	powerup_sharp_turn = false;
	powerup_mirror = false;
}

void Worm::set_powerup_timer()
{
	powerup_timer = 5000;
}
