#include "FasterMe.h"
#include "Powerup.h"
#include <vector>

using namespace std;

FasterMe::FasterMe(Position_class new_position, Uint32 color)
:Powerup(new_position, color), affect_map(false)
{}

void FasterMe::execute(int index, vector<Worm>& Worm_vector)
{
	Worm_vector[index].reset_worm();
	Worm_vector[index].speed = 1.5*Worm_vector[index].speed;
	Worm_vector[index].set_powerup_timer();
}
