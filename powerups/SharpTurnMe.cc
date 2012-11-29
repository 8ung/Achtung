#include "SharpTurnMe.h"
#include "Powerup.h"
#include <vector>

using namespace std;

SharpTurnMe::SharpTurnMe(Position_class new_position, Uint32 color)
:Powerup(new_position, color), affect_map(false)
{}

void SharpTurnMe::execute(int index, vector<Worm>& Worm_vector)
{
	Worm_vector[index].reset_worm();
	Worm_vector[index].powerup_sharp_turn = true;
	Worm_vector[index].set_powerup_timer();
}
