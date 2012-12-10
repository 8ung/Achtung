#include "ThroughWallMe.h"
#include "Powerup.h"
#include <vector>


using namespace std;

ThroughWallMe::ThroughWallMe(Position_class new_position, Uint32 color)
:Powerup(new_position, color,
		"C:/Users/Oscar/Workspace/Projektet/src/Bilder/through_walls_power.bmp"),
		affect_map(false)
{}

void ThroughWallMe::execute(int, vector<Worm*>& Worm_vector)
{
	int vector_size = Worm_vector.size();
	for(int index = 0; index == vector_size - 1; index++)
	{
			Worm_vector[index]->reset_worm();
			Worm_vector[index]->powerup_through_wall = true;
			Worm_vector[index]->set_powerup_timer();
	}

}
