#include "SlowerMe.h"
#include "Powerup.h"
#include <vector>

using namespace std;

SlowerMe::SlowerMe(Position_class new_position, Uint32 color)
:Powerup(new_position, color,
		"slower_me_power.bmp"),
		affect_map(false)
{}

/*execute anropas d� en mask k�r in i en slower_me-powerup. Den g�r igenom survivor_vector (de maskar som
fortfarande lever) och �ndrar double-variabeln "speed" till 0.5 av det ursprungliga v�rdet.
Detta utf�rs i den mask som tog powerupen dvs den mask som har "index" som placering i "survivor_vector".*/
void SlowerMe::execute(int index, vector<Worm*>& Worm_vector)
{
	Worm_vector[index]->reset_worm();
	Worm_vector[index]->speed = 0.5*Worm_vector[index]->speed;
	Worm_vector[index]->set_powerup_timer();
}
