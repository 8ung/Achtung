#include "Worm.h"
#include <math.h>
#include <time.h>

Worm::Worm(const Uint32 new_colour, const int new_left_control,
		const int new_right_control,const int window_height)
{
	distance_to_hole = random_distance_to_hole();
	colour = new_colour;
	left_control = new_left_control;
	right_control = new_right_control;
	position = new Position_class();
	position->random_position(window_height);
	direction = rand() % 360;
	if(colour > 70000)		// v�rden �ver 70000 r�knas som en varm f�rg
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
	int distance = rand() % 5000;	// 5000 �r ett avv�gt max antal loopar mellan tv� h�l
	return distance;
}

Worm::~Worm()
{
	delete[] position;
}

const Uint32 Worm::get_colour()
{
	return colour;
}

const int Worm::get_left_control()
{
	return left_control;
}

const int Worm::get_right_control()
{
	return right_control;
}

/*change_direction anropas d� en mask ska sv�nga. Om det �r s� att v�rdet f�r "direction" g�r �ver 360 s� �ndras
 * v�rdet till 0. Om det �r s� att "direction" g�r under noll s� �ndras den ist�llet till 360.
 */
void Worm::change_direction(const double degrees)
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

const int Worm::get_direction()
{
	return direction;
}

void Worm::set_position(const double x, const double y)
{
	position->x_koord = x;
	position->y_koord = y;
}

/*move anv�der "direction" och "speed" f�r att ber�kna maskens nya position.*/
void Worm::move()
{
	if(distance_to_hole == -350)	// 350 �r antalet loopar som masken r�knas som ett h�l
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

void Worm::random_position(const int window_height)
{
	position->random_position(window_height);
}

void Worm::kill_worm()
{
	speed = 0;
}

void Worm::add_score()
{
	score++;
}

const int Worm::get_score()
{
	return score;
}

const int Worm::get_distance_to_hole()
{
	return distance_to_hole;
}

void Worm::set_powerup_timer()
{
	powerup_timer = 5000; // 5000 �r antal loopar som powerupen verkar
}

void Worm::reset_worm()
{
	speed = default_speed;
	thickness = default_thickness;
	powerup_timer = default_powerup_timer;
	powerup_through_wall = false;
	powerup_mirror = false;
	distance_to_hole = 0;
}
