#include "MirrorYou.h"
#include "Powerup.h"
#include <vector>

using namespace std;

MirrorYou::MirrorYou(Position_class new_position, Uint32 color)
:Powerup(new_position, color, "mirror_you_power.bmp"), affect_map(false)
{}

void MirrorYou::execute(int index, vector<Worm*>& Worm_vector)
{
	int vector_size = Worm_vector.size();
	for(int i = 0; i < vector_size; i++)
		if(index != i)
		{
			Worm_vector[i]->reset_worm();
			Worm_vector[i]->powerup_mirror = true;
			Worm_vector[i]->set_powerup_timer();
		}
}
