#include "FasterMe.h"
#include "Powerup.h"
#include <vector>
#include "SDL_video.h"

using namespace std;

FasterMe::FasterMe(const Position_class new_position, const Uint32 color)
:Powerup(new_position, color, "faster_me_power.bmp"),
 affect_map(false)
{}

/*execute anropas då en mask kör in i en faster_me-powerup. Den går igenom survivor_vector (de maskar som
fortfarande lever) och ändrar double-variabeln "speed" till 2 gånger det ursprungliga värdet.
Detta utförs i den mask som tog powerupen dvs den mask som har "index" som placering i "survivor_vector".*/
void FasterMe::execute(const int index, vector<Worm*>& survivor_vector)
{
	survivor_vector[index]->reset_worm();
	survivor_vector[index]->speed = 2*survivor_vector[index]->speed;
	survivor_vector[index]->set_powerup_timer();
}
