#include "Playground.h"
#include <time.h>
#include <math.h>
#include <vector>

Playground::Playground(int window_height) {
	upper_left_corner = new Position_class(0, 0);
	bottom_right_corner = new Position_class(window_height,window_height);
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
	for(int i = 0; i < worm_vector_size; i++)
	{
		worm_vector[i]->random_position(bottom_right_corner->x_koord);
		//srand(time(NULL));
		int angle = rand() % 360;
		worm_vector[i]->change_direction(angle);
		worm_vector[i]->reset_worm();
	}
	survivor_vector = worm_vector;
}

void Playground::random_power_up_values() {
	throw "Not yet implemented";
}

void Playground::sort_vectors()
{
	int worm_vector_size = worm_vector.size();
	std::vector<Worm*> temp_worm_vector;
	Worm* temp_worm = NULL;
	for(int worm_index = 0; worm_index < worm_vector_size -1; worm_index ++)
	{
		int next_worm_index = worm_index + 1;
		if(worm_vector[worm_index]->get_score() < worm_vector[next_worm_index]->get_score())
		{
			temp_worm = worm_vector[worm_index];
			worm_vector[worm_index] = worm_vector[next_worm_index];
			worm_vector[next_worm_index] = temp_worm;
		}
	}
}

bool Playground::game_finsished(bool teamplay)
{
	if(teamplay)
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
			if((teamhot >= 10 && teamhot > teamcold + 2) ||
					(teamcold >= 10 && teamcold > teamhot + 2))
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
					(worm_vector[0]->get_score() > 10*worm_vector_size);
	}
}

void Playground::collision(SDL_Surface* display)
{
	int survivor_vector_size = survivor_vector.size();
	for(int worm_index = 0; worm_index < survivor_vector_size; worm_index++)
	{

		if(survivor_vector[worm_index]->get_distance_to_hole() > 0)
		{
			double center_x = survivor_vector[worm_index]->get_position()->x_koord;
			double center_y = survivor_vector[worm_index]->get_position()->y_koord;
			double thickness = survivor_vector[worm_index]->thickness + 1.3;
			double angle = survivor_vector[worm_index]->get_direction();
			Uint32 bg_color = SDL_MapRGB(display->format, 0, 0, 0);

			bool right = get_pixel(display, center_x + thickness*cos((angle - 30)*3.141592/180),
					center_y + thickness*sin((angle-30)*3.141592/180)) != bg_color;
			bool left = get_pixel(display, center_x + thickness*cos((angle + 30)*3.141592/180),
					center_y + thickness*sin((angle+30)*3.141592/180)) != bg_color;
			if(right || left)
			{
				survivor_vector[worm_index]->kill_worm();
				survivor_vector.erase(survivor_vector.begin() + worm_index);
				for(int index = 0; index < survivor_vector_size-1; index++)
				{
					survivor_vector[index]->add_score();
				}
				sort_vectors();
			}
		}
		else if(survivor_vector[worm_index]->get_position()->x_koord < 5 ||
				survivor_vector[worm_index]->get_position()->y_koord < 5 ||
				survivor_vector[worm_index]->get_position()->x_koord > bottom_right_corner->x_koord - 5 ||
				survivor_vector[worm_index]->get_position()->y_koord > bottom_right_corner->y_koord - 5)
		{
			survivor_vector[worm_index]->kill_worm();
			survivor_vector.erase(survivor_vector.begin() + worm_index);
			for(int index = 0; index < survivor_vector_size-1; index++)
			{
				survivor_vector[index]->add_score();
			}
			sort_vectors();
		}
	}
}

bool Playground::round_finished() {
	if(survivor_vector.size() <= 1)
	{
		return true;
	}
	return false;
}

void Playground::update(int worm_index, bool left_bool, bool right_bool)
{
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
