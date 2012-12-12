#include "FasterYou.h"
#include "Powerup.h"
#include <vector>

using namespace std;

FasterYou::FasterYou(Position_class new_position, Uint32 color)
:Powerup(new_position, color,
		"faster_you_power.bmp"),
		affect_map(false)
{}

/*execute anropas då en mask kör in i en faster_you-powerup. Den går igenom survivor_vector (de maskar som
fortfarande lever) och ändrar double-variabeln "speed" till 2 gånger det ursprungliga värdet.
Detta utförs i alla maskar med undantag för den som tog powerupen dvs den mask som har "index" som placering
i "survivor_vector".*/
void FasterYou::execute(int index, vector<Worm*>& Worm_vector)
{
	int vector_size = Worm_vector.size();
	for(int i = 0; i < vector_size; i++)
		if(index != i)
		{
			Worm_vector[i]->reset_worm();
			Worm_vector[i]->speed = 2*Worm_vector[i]->speed;
			Worm_vector[i]->set_powerup_timer();
		}
}
