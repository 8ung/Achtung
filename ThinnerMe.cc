#include "ThinnerMe.h"
#include "Powerup.h"
#include <vector>

using namespace std;

ThinnerMe::ThinnerMe(Position_class new_position, Uint32 color)
:Powerup(new_position, color, "thinner_me_power.bmp"), affect_map(false)
{}

void ThinnerMe::execute(int index, vector<Worm*>& Worm_vector)
{
			Worm_vector[index]->reset_worm();
			Worm_vector[index]->thickness = 0.6*Worm_vector[index]->thickness;
			Worm_vector[index]->set_powerup_timer();
}
