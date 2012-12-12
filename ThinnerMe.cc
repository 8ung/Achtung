#include "ThinnerMe.h"
#include "Powerup.h"
#include <vector>

using namespace std;

ThinnerMe::ThinnerMe(Position_class new_position, Uint32 color)
:Powerup(new_position, color, "thinner_me_power.bmp"), affect_map(false)
{}

/*execute anroaps då en mask kör in i en thinner_me-powerup. Den går igenom survivor_vector (de maskar som
fortfarande lever) och ändrar double-variabeln "thickness" till 0.6 av det ursprungliga värdet.
Detta utförs bara i den mask som tog powerupen dvs den mask som har "index" som position i "survivor_vector".*/
void ThinnerMe::execute(int index, vector<Worm*>& Worm_vector)
{
			Worm_vector[index]->reset_worm();
			Worm_vector[index]->thickness = 0.6*Worm_vector[index]->thickness;
			Worm_vector[index]->set_powerup_timer();
}
