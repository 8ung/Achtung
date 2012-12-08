#ifndef PLAYGROUND_H
#define PLAYGROUND_H

#include <vector>
#include "Worm.h"
#include "SDL.h"
#include "Powerup.h"


class Playground {

private:
	const double turn_ratio = 0.2;
	const double sharp_turn = 90;
	int time_to_next_powerup = 0;
public:
	Position_class* upper_left_corner;
	Position_class* bottom_right_corner;
	std::vector<Powerup*> powerup_vector;
	std::vector<Worm*> worm_vector;
	std::vector<Worm*> survivor_vector;
	Powerup* powerup_to_draw;
	Powerup* powerup_to_erase;
	bool round_finished;
	int test_variable;

	Playground(int);
	//~Playground();

private:

	Uint32 get_pixel(SDL_Surface*, double, double);

	void random_powerup_values();

	void sort_vectors(bool);

	void team_collision();

public:

	void start_new_round();

	bool game_finished(bool);

	void collision(SDL_Surface*, bool);

	void update(int,bool,bool);

	void reset();

	void initialize(Uint32 colour, unsigned int left_control, unsigned int right_control);
};

#endif /* PLAYGROUND_H */
