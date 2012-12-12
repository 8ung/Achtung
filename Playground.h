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
	Position_class* upper_left_corner;
	Position_class* bottom_right_corner;

	Uint32 get_pixel(const SDL_Surface*, const double, const double);

	void random_powerup_values();

	void sort_vectors(const bool);

	const bool collision_with_powerup(const int, const Uint32, const Uint32);

	void through_wall(const int, const double, const double);

	void team_collision();


public:
	std::vector<Powerup*> powerup_vector;
	std::vector<Worm*> worm_vector;
	std::vector<Worm*> survivor_vector;
	Powerup* powerup_to_draw;
	Powerup* powerup_to_erase;
	bool round_finished;

	Playground(const int);

	void initialize(const Uint32, const unsigned int, const unsigned int);

	void update(const int, const bool, const bool);

	void collision(const SDL_Surface*, const bool);

	void start_new_round();

	const bool game_finished(const bool);

	void reset();

};

#endif /* PLAYGROUND_H */
