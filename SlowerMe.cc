#include "SlowerMe.h"
#include "Powerup.h"
#include <vector>

using namespace std;

SlowerMe::SlowerMe(Position_class new_position, Uint32 color)
:Powerup(new_position, color,
		"slower_me_power.bmp"),
		affect_map(false)
{}

void SlowerMe::execute(int index, vector<Worm*>& Worm_vector)
{
	Worm_vector[index]->reset_worm();
	Worm_vector[index]->speed = 0.5*Worm_vector[index]->speed;
	Worm_vector[index]->set_powerup_timer();
}
