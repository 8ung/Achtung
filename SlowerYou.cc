#include "SlowerYou.h"
#include "Powerup.h"
#include <vector>

using namespace std;

SlowerYou::SlowerYou(const Position_class new_position, const Uint32 color)
:Powerup(new_position, color,
		"slower_you_power.bmp"),
		affect_map(false)
{}

/*execute anropas d� en mask k�r in i en slower_you-powerup. Den g�r igenom survivor_vector (de maskar som
fortfarande lever) och �ndrar double-variabeln "speed" till 0.5 av det ursprungliga v�rdet.
Detta utf�rs i alla maskar med undantag f�r den som tog powerupen dvs den mask som har "index" som placering
i "survivor_vector".*/
void SlowerYou::execute(const int index, vector<Worm*>& Worm_vector)
{
	int vector_size = Worm_vector.size();
	for(int i = 0; i < vector_size; i++)
		if(index != i)
		{
			Worm_vector[i]->reset_worm();
			Worm_vector[i]->speed = 0.5*Worm_vector[i]->speed;
			Worm_vector[i]->set_powerup_timer();
		}
}
