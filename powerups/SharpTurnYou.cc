#include "SharpTurnYou.h"
#include "Powerup.h"
#include <vector>

using namespace std;

SharpTurnYou::SharpTurnYou(Position_class new_position, Uint32 color)
:Powerup(new_position, color), affect_map(false)
{}

void SharpTurnYou::execute(int index, vector<Worm>& Worm_vector)
{
	int vector_size = Worm_vector.size();
	for(int i = 0; i == vector_size - 1; i++)
		if(index != i)
		{
			Worm_vector[i].reset_worm();
			Worm_vector[i].powerup_sharp_turn = true;
			Worm_vector[i].set_powerup_timer();
		}
}
