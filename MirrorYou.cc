#include "MirrorYou.h"
#include "Powerup.h"
#include <vector>

using namespace std;

MirrorYou::MirrorYou(const Position_class new_position, const Uint32 color)
:Powerup(new_position, color, "mirror_you_power.bmp"), affect_map(false)
{}

/*execute anropas d� en mask k�r in i en mirror_you-powerup. Den g�r igenom survivor_vector (de maskar som
fortfarande lever) och �ndrar den boolska variabeln "powerup_mirror" till true. Detta utf�rs i alla maskar med
undantag f�r den som tog powerupen dvs den mask som har "index" som placering i "survivor_vector".*/
void MirrorYou::execute(const int index, vector<Worm*>& Worm_vector)
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
