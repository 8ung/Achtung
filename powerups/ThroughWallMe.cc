#include "ThroughWallMe.h"
#include "Powerup.h"
#include <vector>

using namespace std;

ThroughWallMe::ThroughWallMe(Position_class new_position, Uint32 color)
:Powerup(new_position, color), affect_map(false)
{}

void ThroughWallMe::execute(int index, vector<Worm>& Worm_vector)
{
			Worm_vector[index].reset_worm();
			Worm_vector[index].powerup_through_wall = true;
			Worm_vector[index].set_powerup_timer();
}
