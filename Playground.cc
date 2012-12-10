#include "Playground.h"
#include <time.h>
#include <math.h>
#include <vector>
#include "Powerup.h"
#include "FasterMe.h"
#include "FasterYou.h"
#include "SlowerMe.h"
#include "SlowerYou.h"
#include "ThroughWallMe.h"

Playground::Playground(int window_height) {
	upper_left_corner = new Position_class(0, 0);
	bottom_right_corner = new Position_class(window_height,window_height);
	round_finished = false;
	time_to_next_powerup = 0;
	powerup_to_draw = NULL;
	powerup_to_erase = NULL;
	test_variable = 0;
}

Uint32 Playground::get_pixel(SDL_Surface* display, double x, double y)
{
	int bpp = display->format->BytesPerPixel;
	/* Here p is the address to the pixel we want to retrieve */
	Uint8 *p = (Uint8 *)display->pixels + (int)y * display->pitch + (int)x * bpp;

	switch(bpp) {
	case 1:
		return *p;
		break;

	case 2:
		return *(Uint16 *)p;
		break;

	case 3:
		if(SDL_BYTEORDER == SDL_BIG_ENDIAN)
			return p[0] << 16 | p[1] << 8 | p[2];
		else
			return p[0] | p[1] << 8 | p[2] << 16;
		break;

	case 4:
		return *(Uint32 *)p;
		break;

	default:
		return 0;       /* shouldn't happen, but avoids warnings */
	}
}

void Playground::start_new_round()
{
	int worm_vector_size  = worm_vector.size();
	round_finished = false;
	for(int i = 0; i < worm_vector_size; i++)
	{
		worm_vector[i]->random_position(bottom_right_corner->x_koord);
		//srand(time(NULL));
		int angle = rand() % 360;
		worm_vector[i]->change_direction(angle);
		worm_vector[i]->reset_worm();
	}
	int powerup_vector_size = powerup_vector.size();
	for(int powerup_index = 0; powerup_index < powerup_vector_size; powerup_index++)
	{
		powerup_vector.erase(powerup_vector.begin());
	}
	survivor_vector = worm_vector;
}

void Playground::random_powerup_values()
{
	if(time_to_next_powerup <= 0)
	{
		time_to_next_powerup = rand() % 30000;
		bool taken_colour = true;
		Uint32 powerup_colour;
		while (taken_colour)
		{
			taken_colour = false;
			powerup_colour = rand() % 16777215;
			int worm_vector_size = worm_vector.size();
			for(int worm_vector_index = 0; worm_vector_index < worm_vector_size; worm_vector_index++)
			{
				if(worm_vector[worm_vector_index]->get_colour() == powerup_colour)
				{
					taken_colour = true;
				}
			}
			int powerup_vector_size = powerup_vector.size();
			for(int powerup_vector_index = 0; powerup_vector_index < powerup_vector_size; powerup_vector_index++)
			{
				if(powerup_vector[powerup_vector_index]->colour_id == powerup_colour)
				{
					taken_colour = true;
				}
			}
		}
		Position_class powerup_position;
		powerup_position.random_position(bottom_right_corner->x_koord);
		int random_powerup = rand() % 5;
		switch (random_powerup)
		{
		case 0:
			powerup_to_draw = new FasterMe(powerup_position, powerup_colour);
			break;
		case 1:
			powerup_to_draw = new FasterYou(powerup_position, powerup_colour);
			break;
		case 2:
			powerup_to_draw = new SlowerMe(powerup_position, powerup_colour);
			break;
		case 3:
			powerup_to_draw = new SlowerYou(powerup_position, powerup_colour);
			break;
		case 4:
			powerup_to_draw = new ThroughWallMe(powerup_position, powerup_colour);
			break;
		}
		powerup_vector.push_back(powerup_to_draw);
	}
	else
	{
		time_to_next_powerup--;
	}
}

void Playground::sort_vectors(bool team_play)
{
	int worm_vector_size = worm_vector.size();
	std::vector<Worm*> temp_worm_vector;
	Worm* temp_worm = NULL;
	bool change = true;
	while(change == true)
	{
		change = false;
		for(int worm_index = 0; worm_index < worm_vector_size -1; worm_index ++)
		{
			int next_worm_index = worm_index + 1;
			if(worm_vector[worm_index]->get_score() < worm_vector[next_worm_index]->get_score())
			{
				temp_worm = worm_vector[worm_index];
				worm_vector[worm_index] = worm_vector[next_worm_index];
				worm_vector[next_worm_index] = temp_worm;
				change = true;
			}
		}
	}
}

bool Playground::game_finished(bool team_play)
{
	if(team_play)
	{
		int teamhot = -1;
		int teamcold = -1;
		int counter = 0;
		while(teamhot == -1 || teamcold == -1)
		{
			if(worm_vector[counter]->team == "hot")
			{
				teamhot = worm_vector[counter]->get_score();
			}
			else if(worm_vector[counter]->team == "cold")
			{
				teamcold = worm_vector[counter]->get_score();
			}
			counter++;
		}
		if(((teamhot >= 10) && (teamhot > teamcold + 2)) ||
				((teamcold >= 10) && (teamcold > teamhot + 2)))
		{
			return true;
		}
		else
		{
			return false;
		}
	}
	else
	{
		int worm_vector_size = worm_vector.size();
		return ((worm_vector[0]->get_score() - worm_vector[1]->get_score()) > 1) &&
				(worm_vector[0]->get_score() >= 10*(worm_vector_size-1));
	}
}

void Playground::team_collision()
{
	int cold_worms_alive = 0;
	int hot_worms_alive = 0;
	int surv_size = survivor_vector.size();
	for(int survivor_vector_index = 0;
			survivor_vector_index < surv_size; survivor_vector_index++)
	{
		if(survivor_vector[survivor_vector_index]->team == "cold")
		{
			cold_worms_alive++;
		}
		else
		{
			hot_worms_alive++;
		}
	}
	if(cold_worms_alive <= 0 || hot_worms_alive <= 0)
	{
		std::string winner = survivor_vector[0]->team;
		int worm_vector_size = worm_vector.size();
		for(int worm_vector_index = 0;
				worm_vector_index < worm_vector_size; worm_vector_index++)
		{
			if(worm_vector[worm_vector_index]->team == winner)
			{
				worm_vector[worm_vector_index]->add_score();
			}
		}
		round_finished = true;
	}
}

void Playground::collision(SDL_Surface* display, bool team_play)
{
	int survivor_vector_size = survivor_vector.size();
	int worm_index = 0;
	//for(int worm_index = 0; worm_index < survivor_vector_size; worm_index++)
	while(worm_index < survivor_vector_size)
	{
		double thickness = survivor_vector[worm_index]->thickness + 1.3;
		double angle = survivor_vector[worm_index]->get_direction();
		double center_x = survivor_vector[worm_index]->get_position()->x_koord;
		double center_y = survivor_vector[worm_index]->get_position()->y_koord;
		Uint32 right_pixel_colour = get_pixel(display, center_x + thickness*cos((angle - 30)*3.141592/180),
				center_y + thickness*sin((angle-30)*3.141592/180));
		Uint32 left_pixel_colour = get_pixel(display, center_x + thickness*cos((angle + 30)*3.141592/180),
				center_y + thickness*sin((angle+30)*3.141592/180));
		int offset = 17;
		if(survivor_vector[worm_index]->get_distance_to_hole() > 0)
		{
			Uint32 bg_color = SDL_MapRGB(display->format, 0, 0, 0);

			bool right = get_pixel(display, center_x + thickness*cos((angle - 30)*3.141592/180),
					center_y + thickness*sin((angle-30)*3.141592/180)) != bg_color;
			bool left = get_pixel(display, center_x + thickness*cos((angle + 30)*3.141592/180),
					center_y + thickness*sin((angle+30)*3.141592/180)) != bg_color;
			if(right || left)
			{
				bool powerup_bool = true;
				int powerup_vector_index = 0;
				int powerup_vector_size = powerup_vector.size();
				while(powerup_vector_index < powerup_vector_size)
				{
					if(right_pixel_colour == powerup_vector[powerup_vector_index]->colour_id ||
							left_pixel_colour == powerup_vector[powerup_vector_index]->colour_id)
					{
						powerup_vector[powerup_vector_index]->execute(worm_index, survivor_vector);
						powerup_to_erase = powerup_vector[powerup_vector_index];
						//powerup_vector[powerup_vector_index] = nullptr;
						//std::vector<Powerup*>::iterator it;
						//it = remove(powerup_vector.begin(),powerup_vector.end(),nullptr);
						//delete powerup_vector[powerup_vector_index];
						//powerup_vector.erase(powerup_vector.begin() + powerup_vector_index);
						powerup_bool = false;
						powerup_vector_index = powerup_vector_size;
					}
					powerup_vector_index++;
				}
				if(survivor_vector[worm_index]->powerup_through_wall &&
						((left_pixel_colour == 16777215) || (right_pixel_colour == 16777215)))
				{
					if(center_x < offset)
					{
						survivor_vector[worm_index]->set_position(bottom_right_corner->x_koord - offset, center_y);
					}
					else if(center_x > bottom_right_corner->x_koord - offset)
					{
						survivor_vector[worm_index]->set_position(offset, center_y);
					}
					else if(center_y < offset)
					{
						survivor_vector[worm_index]->set_position(center_x, bottom_right_corner->y_koord - offset);
					}
					else if(center_y > bottom_right_corner->y_koord - offset)
					{
						survivor_vector[worm_index]->set_position(center_x, offset);
					}
				}
				else if(powerup_bool)
				{
					survivor_vector[worm_index]->kill_worm();
					survivor_vector.erase(survivor_vector.begin() + worm_index);
					if(team_play)
					{
						team_collision();
					}
					else
					{
						for(int index = 0; index < survivor_vector_size -1; index++)
						{
							survivor_vector[index]->add_score();
						}
						int size = survivor_vector.size();
						if(size <= 1)
						{
							round_finished = true;
						}
					}
				}
			}
		}
		else if(survivor_vector[worm_index]->get_position()->x_koord < 10 ||
				survivor_vector[worm_index]->get_position()->y_koord < 10 ||
				survivor_vector[worm_index]->get_position()->x_koord > bottom_right_corner->x_koord - 10 ||
				survivor_vector[worm_index]->get_position()->y_koord > bottom_right_corner->y_koord - 10)
		{
			if(//survivor_vector[worm_index]->powerup_through_wall &&
					((left_pixel_colour == 16777215) || (right_pixel_colour == 16777215)))
			{
				if(center_x < offset)
				{
					survivor_vector[worm_index]->set_position(bottom_right_corner->x_koord - offset, center_y);
				}
				else if(center_x > bottom_right_corner->x_koord - offset)
				{
					survivor_vector[worm_index]->set_position(offset, center_y);
				}
				else if(center_y < offset)
				{
					survivor_vector[worm_index]->set_position(center_x, bottom_right_corner->y_koord - offset);
				}
				else if(center_y > bottom_right_corner->y_koord - offset)
				{
					survivor_vector[worm_index]->set_position(center_x, offset);
				}
			}
			else
			{
				survivor_vector[worm_index]->kill_worm();
				survivor_vector.erase(survivor_vector.begin() + worm_index);
				if(team_play)
				{
					team_collision();
				}
				else
				{
					for(int index = 0; index < survivor_vector_size-1; index++)
					{
						survivor_vector[index]->add_score();
					}
				}
			}


		}
		sort_vectors(team_play);
		worm_index++;
		survivor_vector_size = survivor_vector.size();
	}
}

void Playground::update(int worm_index, bool left_bool, bool right_bool)
{
	random_powerup_values();
	//survivor_vector[worm_index]->powerup_sharp_turn = true;
	double degrees = 0;
	int mirror_degrees = 0;
	if(survivor_vector[worm_index]->powerup_sharp_turn == true)
	{
		if(SDL_GetTicks() % 90 == 1)
		{
			degrees = sharp_turn;
		}
	}
	else
	{
		degrees = turn_ratio;
	}
	if(survivor_vector[worm_index]->powerup_mirror == true)
	{
		mirror_degrees = -1;
	}
	else
	{
		mirror_degrees = 1;
	}
	if(left_bool)
	{
		survivor_vector[worm_index]->change_direction(mirror_degrees * degrees);
		survivor_vector[worm_index]->move();
	}
	else if(right_bool)
	{
		survivor_vector[worm_index]->change_direction(- mirror_degrees * degrees);
		survivor_vector[worm_index]->move();
	}
	else
	{
		survivor_vector[worm_index]->move();
	}
}

void Playground::initialize(Uint32 colour,
		unsigned int left_control,
		unsigned int right_control)
{
	Worm* temp_worm = new Worm(colour, left_control, right_control, bottom_right_corner->x_koord);
	worm_vector.push_back(temp_worm);
	survivor_vector.push_back(temp_worm);
}

void Playground::reset()
{
	int worm_vector_size  = worm_vector.size();
	for(int worm_index = 0; worm_index < worm_vector_size; worm_index++)
	{
		delete worm_vector[0];
		worm_vector.erase(worm_vector.begin());
	}
	survivor_vector = worm_vector;
}
