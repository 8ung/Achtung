#include "FasterMe.h"
#include "Powerup.h"
#include <vector>
#include "SDL_video.h"

using namespace std;

FasterMe::FasterMe(Position_class new_position, Uint32 color)
:Powerup(new_position, color, "faster_me_power.bmp"),
 affect_map(false)
{}

void FasterMe::execute(int index, vector<Worm*>& survivor_vector)
{
	survivor_vector[index]->reset_worm();
	survivor_vector[index]->speed = 2*survivor_vector[index]->speed;
	survivor_vector[index]->set_powerup_timer();
}
